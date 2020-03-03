// rex.c - Recognizer for Tog compiler - Jim Hogg, 2019

#include "rex.h"

// Body => Var* Stm+
// ============================================================================
void rexBody(Toks* toks) {
  rexVars(toks);
  rexStms(toks);
}


// Call => "call" Nam NamLit*
//
// Eg: call add3 x 15 y
// ============================================================================
void rexCall(Toks* toks) {
  rexMust(toks, TOKCALL);                           // "call"
  rexMust(toks, TOKNAM);                            // eg: "add3"
  TokKind k = toksCurr(toks)->kind;
  if (! rexStmFollower(k)) {
    rexMust3(toks, TOKNAM, TOKNUMLIT, TOKSTRLIT);   // eg: "x"
    k = toksCurr(toks)->kind;
    while (! rexStmFollower(k)) {
      rexMust3(toks, TOKNAM, TOKNUMLIT, TOKSTRLIT); // eg: "15"   "y"
      k = toksCurr(toks)->kind;
    }
  }
}


// Exp => NamLit | NamLit Bop NamLit | Call
//
// Eg: 'hello'   | x + 23            | call add3 x 15 y
// ============================================================================
void rexExp(Toks* toks) {
  rexMust4(toks, TOKNUMLIT, TOKNAM, TOKSTRLIT, TOKCALL);
  Tok* tok = toksPrev(toks);

  if (tok->kind == TOKCALL)   {                 // "call" Nam NamLit*
    rexCall(toks);
    return;
  }

  if (tok->kind == TOKSTRLIT) {                 // eg: 'hello'
    rexStr(toks);
    return;
  }

  rexMust3(toks, TOKNAM, TOKNUMLIT, TOKSTRLIT);
  Tok* bop = toksCurr(toks);
  if (! rexIsBop(bop->kind) ) return;           // just NamLit

  toksNext(toks);                               // skip bop
  rexMust3(toks, TOKNAM, TOKNUMLIT, TOKSTRLIT);
}


// Fun => "fun" Nam ":" Type Param+ "=" Body "nuf"
//      | "main" "=" Body "nuf"
//
// Eg: fun add:num a:num b:num =
//
// Note that we treat function "main" as a special case - no return
// type and no params.  Ie: "fun main = "
// ============================================================================
void rexFun(Toks* toks) {
  rexMust(toks, TOKFUN);                    // eg: fun
  Tok* tok = toksCurr(toks);                // eg: add
  rexMust(toks, TOKNAM);                    // eg: add
  if (strcmp(tok->lexeme, "main") == 0) {   // "main" function
    rexMust(toks, TOKEQ);
  } else {
    rexMust(toks, TOKCOLON);                // eg: :
    rexMust2(toks, TOKNUM, TOKSTR);         // eg: TOKNUM
    rexParams(toks);                        // eg: a:num  b:num
    rexMust(toks, TOKEQ);
  }

  rexBody(toks); 
  rexMust(toks, TOKNUF);                    // eg: nuf
}

// If => "if" Exp "then" Stm+ "fi"
// ============================================================================
void rexIf(Toks* toks) {
  //Should take 5 lines (?)
  rexMust(toks, TOKIF); 
  rexExp(toks); 
  rexMust(toks, TOKTHEN); 
  rexStms(toks); 
  rexMust(toks, TOKFI); 
}


// Check whether the Token kind specified by 'k' is legal binary operator
// ============================================================================
int rexIsBop(TokKind k) {
  return (k == TOKADD || k == TOKSUB || k == TOKMUL || k == TOKDIV ||
          k == TOKLE  || k == TOKLT  || k == TOKNE  || k == TOKEEQ ||
          k == TOKGE  || k == TOKGT  || k == TOKLE  || k == TOKLT  ||
          k == TOKNE  || k == TOKEEQ || k == TOKGE  || k == TOKGT);
}


// Let => "let" Nam "=" Exp
// eg: let count = a + 4;
// eg: let hi = 'hello world'
// ============================================================================
void rexLet(Toks* toks) {
  rexMust(toks, TOKLET);        // eg: let
  rexMust(toks, TOKNAM);        // eg: count
  rexMust(toks, TOKEQ);         // eg: =
  rexExp(toks);                 // eg: a + 4
}


// Check that the kind of the current Token within 'toks' matches 'k1'.
// If yes, advance to the next Token in 'toks'.  If not, abort the program.
// ============================================================================
void rexMust(Toks* toks, TokKind k1) {
  Tok* tok = toksCurr(toks);              // current token
  TokKind k = tok->kind;
  if (k == k1) {                          // success!
    toksNext(toks);                       // advance to next token
    return;
  }

  char msg[150];
  sprintf(msg, "but expecting {%s}", tokStr(k1));
  utDieStrTokStr("rexMust", tok, msg);
}


// Check that the kind of the current Token within 'toks' matches 'k1' or 'k2'.
// If yes, advance to the next Token in 'toks'.  If not, abort the program.
// ============================================================================
void rexMust2(Toks* toks, TokKind k1, TokKind k2) {
  Tok* tok = toksCurr(toks);              // current token
  TokKind k = tok->kind;
  if (k == k1 || k == k2) {               // success!
    toksNext(toks);                       // advance to next token
    return;
  }

  char msg[150];
  sprintf(msg, "%s%s, %s}", "but expecting {", tokStr(k1), tokStr(k2));
  utDieStrTokStr("rexMust", tok, msg);
}


// Check that the kind of the current Token within 'toks' matches 'k1' or 'k2'
// or 'k3'.  If yes, advance to the next Token in 'toks'.  If not, abort
// the program.
// ============================================================================
void rexMust3(Toks* toks, TokKind k1, TokKind k2, TokKind k3) {
  Tok* tok = toksCurr(toks);              // current token
  TokKind k = tok->kind;
  if (k == k1 || k == k2 || k == k3) {    // success!
    toksNext(toks);                       // advance to next token
    return;
  }

  char msg[150];
  sprintf(msg, "%s%s, %s, %s}", "but expecting {", tokStr(k1), tokStr(k2), tokStr(k3));
  utDieStrTokStr("rexMust", tok, msg);
}


// Check that the kind of the current Token within 'toks' matches 'k1' thru
// 'k4'.  If yes, advance to the next Token in 'toks'.  If not, abort
// the program.
// ============================================================================
void rexMust4(Toks* toks, TokKind k1, TokKind k2, TokKind k3, TokKind k4) {
  Tok* tok = toksCurr(toks);              // current token
  TokKind k = tok->kind;
  if (k == k1 || k == k2 || k == k3 || k == k4) {   // success!
    toksNext(toks);                                 // advance to next token
    return;
  }

  char msg[150];
  sprintf(msg, "%s%s, %s, %s, %s}", "but expecting {", tokStr(k1), tokStr(k2), tokStr(k3), tokStr(k4));
  utDieStrTokStr("rexMust", tok, msg);
}


// nl => "nl"
// ============================================================================
void rexNl(Toks* toks) {
  rexMust(toks, TOKNL);
}


// NamLit => Nam | Num | Str
// ============================================================================
void rexNamLit(Toks* toks) {
  rexMust3(toks, TOKNAM, TOKNUMLIT, TOKSTRLIT);
}


// Param  => Nam ":" Type
// eg: msg:str
// ============================================================================
void rexParam(Toks* toks) {
  rexMust(toks, TOKNAM);            // eg: msg
  rexMust(toks, TOKCOLON);          // eg: :
  rexMust2(toks, TOKNUM, TOKSTR);   // eg: TOKSTR
}


// Params => Param*
// eg: count:num
// eg: i:num msg:str top:num
// ============================================================================
void rexParams(Toks* toks) {
  rexParam(toks);                   // first Param.  eg: i:num
  Tok* tok = toksCurr(toks);
  while (tok->kind == TOKNAM) {
    rexParam(toks);                 // subsequent Params.  eg: msg:str
    tok = toksCurr(toks);
  }
}


// Prog => Fun+
// ============================================================================
void rexProg(Toks* toks) {
  rexFun(toks);                               // first function
  while (toks->tokNum <= toks->hiTokNum) {
    rexFun(toks);                             // next function
  }
  printf("INFO: Recognizer: Input Program is valid \n\n");
}


// Reporter function for debugging
// ============================================================================
void rexRep(Toks* toks, char* s) {
  printf("%s", s);
  Tok* tok = toksCurr(toks);
  printf("%s %s \n", tokStr(tok->kind), tok->lexeme);
}


// Ret => "ret" Exp
// ============================================================================
void rexRet(Toks* toks) {
  rexMust(toks, TOKRET);
  rexExp(toks);
}


// Say => "say" Exp
// ============================================================================
void rexSay(Toks* toks) {
  rexMust(toks, TOKSAY);
  rexExp(toks);
}


// Stm => If | Let | Nl | Ret | Say | Stop | While
// ============================================================================
void rexStm(Toks* toks) {
  Tok* tok = toksCurr(toks);
  TokKind k = tok->kind;
  if (k == TOKLET) {
    rexLet(toks);
  } else if (k == TOKIF) {
    rexIf(toks);
  } else if (k == TOKNL) {
    rexNl(toks);
  } else if (k == TOKWHILE) {
    rexWhile(toks);
  } else if (k == TOKRET) {
    rexRet(toks);
  } else if (k == TOKSAY) {
    rexSay(toks);
  } else if (k == TOKSTOP) {
    rexStop(toks);
  } else {
    utDieStrTokStr("rexStm", tok, "a statement");
  }
}


// Test whether the Tok kind 'k' is any of those that can legitimately follow
// an Stm.  This is used to determine when we have finished parsing arguments
// to a function call.  If 'k' can be such a follower, return 1, else 0
// ============================================================================
int rexStmFollower(TokKind k) {
  if (k == TOKELIHW)  return 1;
  if (k == TOKFI)     return 1;
  if (k == TOKIF)     return 1;
  if (k == TOKLET)    return 1;
  if (k == TOKNL)     return 1;
  if (k == TOKNUF)    return 1;
  if (k == TOKRET)    return 1;
  if (k == TOKSAY)    return 1;
  if (k == TOKSTOP)   return 1;
  if (k == TOKWHILE)  return 1;
  return 0;
}

// Stms => Stm+
// ============================================================================
void rexStms(Toks* toks) {
  rexStm(toks);

  TokKind k = toksCurr(toks)->kind;
  while (k != TOKNUF && k != TOKFI && k != TOKELIHW) {
    rexStm(toks);
    if (toksAtEnd(toks)) break;
    Tok* tok = toksCurr(toks);
    k = tok->kind;
  }
}


// Stop => "stop"
// ============================================================================
void rexStop(Toks* toks) {
  rexMust(toks, TOKSTOP);
}


// ============================================================================
void rexStr(Toks* toks) {
  rexMust(toks, TOKSTRLIT);
}


// Var => "var" Nam ":" Type
// eg: var count:num
// ============================================================================
void rexVar(Toks* toks) {
  rexMust(toks, TOKVAR);                // eg: var
  rexMust(toks, TOKNAM);                // eg: count
  rexMust(toks, TOKCOLON);              // eg: :
  rexMust2(toks, TOKNUM, TOKSTR);       // eg: num
}


// Vars => Var*
// ============================================================================
void rexVars(Toks* toks) {
  Tok* tok = toksCurr(toks);
  if (tok->kind != TOKVAR) return;

  rexVar(toks);                         // first "var"
  tok = toksCurr(toks);
  while (tok->kind == TOKVAR) {         // subsequent "var"s
    rexVar(toks);
    tok = toksCurr(toks);
  }
}


// While => "while" Exp "do" Stm+ "elihw"
// eg: while n < 10 do let n = n + 1 elihw
// ============================================================================
void rexWhile(Toks* toks) {
  rexMust(toks, TOKWHILE); 
  rexExp(toks); 
  rexMust(toks, TOKDO); 
  rexStms(toks); 
  rexMust(toks, TOKELIHW);
}
