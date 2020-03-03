// par.c - parser for Tog Interpreter - converts tokens into AST  - Jim Hogg, 2019

#include "par.h"

// Append Ast 'a' onto the chain of Asts 'as', linked via the 'next'
// pointer in the Ast struct
// ============================================================================
void parAppend(Ast* as, Ast* a) {
  Ast** p = &as->next;              // p is alias
  while (*p) p = &((*p)->next);
  *p = a;
}


// Body => Var* Stm+
// ============================================================================
AstBody* parBody(Toks* toks) {
  AstVar* vars = parVars(toks);
  Ast* stms = parStms(toks);
  return astNewBody(vars, stms);
}


// Call => "call" Nam NamLit*
// ============================================================================
/*
typedef struct {
  AST   kind;
  Ast*  next;
  char* nam;
  Ast*  args;
} AstCall;
AstCall* astNewCall(char* nam, Ast* args);

// Test whether the Tok kind 'k' is any of those that can legitimately follow
// an Stm.  This is used to determine when we have finished parsing arguments
// to a function call.  If 'k' can be such a follower, return 1, else 0
// ============================================================================
int parStmFollower(TokKind k) {
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

while (tok->kind == TOKVAR) {         // subsequent "var"s
    AstVar* var = parVar(toks);
    parAppend((Ast*) vars, (Ast*) var);
    tok = toksCurr(toks);
  }
  return vars;
*/
AstCall* parCall(Toks* toks) {
  parMust(toks, 1, TOKCALL); 
  char* nam = parMust(toks, 1, TOKNAM)->lexeme;
  //++ Use the above grammar to complete the body of parCall.
  //++ Note that the argument list can contain zero or more NamLit's.
  //++ So you must detect their end by looking for any Token that denotes
  //++ the end of an Stm (statement).  Specifically, any of:
  //++ TOKELIHW, TOKFI, TOKIF, TOKLET, TOKNL, TOKNUF, TOKRET, TOKSAY,
  //++ TOKSTOP or TOKWHILE.
  //++
  //++ Use parStmFollower (provided) to make this check
  if (parStmFollower(toks->tok->kind) != 1){
    Ast* namlits = parNamLit(toks); 
    while(parStmFollower(toks->tok->kind) != 1){
      Ast* namlit = parNamLit(toks); 
      parAppend((Ast*) namlits, (Ast*) namlit); 
      toksNext(toks); 
    }
    return astNewCall(nam, namlits); 
  }

  return astNewCall(nam, NULL);          // FIXUP
}


// Exp => NamLit | NamLit Bop NamLit | Call
// ============================================================================
AstExp* parExp(Toks* toks) {
  AstExp* exp = astNewExp(NULL, BOPNONE, NULL);
  Tok* tok = parMust(toks, 4, TOKNUMLIT, TOKNAM, TOKSTRLIT, TOKCALL);
  toksPrev(toks);

  if (tok->kind == TOKCALL)   {             // "call" Nam NamLit*
    exp->lhs = (Ast*) parCall(toks);
    return exp;
  }

  if (tok->kind == TOKSTR) {                // Str (string literal)
    exp->lhs = (Ast*) parStr(toks);
    return exp;
  }

  exp->lhs = parNamLit(toks);

  Tok* bop = toksCurr(toks);
  if (! parIsBop(bop->kind) ) return exp;   // NamLit

  exp->bop = TOKtoBOP(bop->kind);           // NamLit Bop NamLit
  toksNext(toks);                           // skip Bop
  exp->rhs = parNamLit(toks);
  return exp;
}


// Fun => "fun" Nam ":" Type Param+ "=" Body "nuf" | "main" "=" Body "nuf"
// eg: fun cat:num a:str b:str =
// Note that we treat function 'main' as a special case - no return
// type and no params.  Eg: fun main =
// ============================================================================
AstFun* parFun(Toks* toks) {
  parMust(toks, 1, TOKFUN);                             // eg: fun
  Tok* tokNam = parMust(toks, 1, TOKNAM);               // eg: cat

  char* funName;                                        // function name.  eg: cat
  TYP funTyp;                                           // function return type. eg: TYPNUM
  AstParam* params = NULL;                              // eg: (a:str), (b:str)
  if (strcmp(tokNam->lexeme, "main") == 0) {            // "main" function
    funName = "main";
    funTyp = TYPNONE;
    params = NULL;
  } else {
    funName = tokNam->lexeme;                           // regular function
    parMust(toks, 1, TOKCOLON);
    Tok* tok = parMust(toks, 2, TOKNUM, TOKSTR);        // eg: TOKNUM
    funTyp = TOKtoTYP(tok);                             // eg: TYPNUM
    params = parParams(toks);                           // eg: (a:str), (b:str)
  }
  parMust(toks, 1, TOKEQ);                              // eg: =
  AstBody* body = parBody(toks);
  parMust(toks, 1, TOKNUF);                             // eg: nuf

  return astNewFun(funName, funTyp, params, body);
}


// If => "if" Cmp "then" Stm+ "fi"
// ============================================================================
/*
typedef struct {
  AST     kind;
  Ast*    next;
  AstExp* exp;
  Ast*    stms;
} AstIf;
AstIf* astNewIf(AstExp* exp, Ast* stms);
*/
AstIf* parIf(Toks* toks) {

  parMust(toks, 1, TOKIF);
  AstExp* exp = parExp(toks); 
  parMust(toks, 1, TOKTHEN); 
  Ast* stms = parStms(toks); 
  parMust(toks, 1, TOKFI); 
  AstIf* ifNode = astNewIf(exp, stms); 
  //++ Use the grammar rule above to complete the body
  //++ of this function.  6 lines of code.  Also, replace
  //++ line commented FIXUP

  return ifNode;        // FIXUP
}


// Check whether the Token kind specified by 'k' is legal binary operator
// ============================================================================
int parIsBop(TokKind k) {
  return (k == TOKADD || k == TOKSUB || k == TOKMUL || k == TOKDIV ||
          k == TOKLE  || k == TOKLT  || k == TOKNE  || k == TOKEEQ ||
          k == TOKGE  || k == TOKGT  || k == TOKLE  || k == TOKLT  ||
          k == TOKNE  || k == TOKEEQ || k == TOKGE  || k == TOKGT);
}


// Let => "let" Nam "=" Exp
// eg: let count = a + 4;
// eg: let hi = 'hello world'
// ============================================================================
AstLet* parLet(Toks* toks) {
  parMust(toks, 1, TOKLET);                       // eg: let
  char* nam = parMust(toks, 1, TOKNAM)->lexeme;   // eg: count
  parMust(toks, 1, TOKEQ);                        // eg: =
  AstExp* exp = parExp(toks);                     // eg: a + 4
  return astNewLet(nam, exp);
}


// Check that the current Token within 'toks' matches any of the 'numk'
// kinds in the varargs list.  If yes, advance to the next Token in 'toks'.
// If not, abort the program
//
// eg: parMust(toks, 2, TOKNAM, TOKNUM)
// In this example, if the current Token does not match TOKNAM or TOKNUM,
// we build a diagnostic text in 'msg' and call dieStrTokStr to print a
// message like:
//    "ERROR: parFun: Found TOKLET but expecting {TOKNAM, TOKNUM}"
// ============================================================================
Tok* parMust(Toks* toks, int numk, ...) {

  // Prepare a failure message, just in case

  va_list argp;
  va_start(argp, numk);
  char msg[300] = "{";
  TokKind k;

  for (int i = 1; i <= numk - 1; ++i) {
    k = va_arg(argp, TokKind);
    strcat(msg, tokStr(k));   strcat(msg, ", ");
  }
  k = va_arg(argp, TokKind);
  strcat(msg, tokStr(k));     strcat(msg, "}");
  va_end(argp);

  // Now process the actual request

  if (toksAtEnd(toks)) {
    Tok* tok = tokNew(TOKBAD, "No more tokens", 0.0, 999, 999);
    utDieStrTokStr("parMust", tok, msg);
  }

  Tok* tok = toksCurr(toks);

  va_start(argp, numk);                   // number of TokKind's
  for (int i = 1; i <= numk; ++i) {
    k = va_arg(argp, TokKind);
    if (tok->kind == k) {
      toksNext(toks);
      return tok;
    }
  }
  va_end(argp);

  // Failed to find a match.  So emit the diagnostic

  utDieStrTokStr("parMust", tok, msg);
  return NULL;
}


// nl => "nl"
// ============================================================================
AstNl* parNl(Toks* toks) {
  parMust(toks, 1, TOKNL);
  return astNewNl();
}


// NamLit => Nam | NumLit | StrLit
// ============================================================================
Ast* parNamLit(Toks* toks) {
  Tok* tok = parMust(toks, 3, TOKNAM, TOKNUMLIT, TOKSTRLIT);
  if (tok->kind == TOKNAM) {
    return (Ast*) astNewNam(tok->lexeme);
  } else if (tok->kind == TOKNUMLIT) {
    return (Ast*) astNewNum(tok->numlit);
  } else {
    return (Ast*) astNewStr(tok->lexeme);
  }
}


// Params => Nam ":" Type
// eg: msg:str
// ============================================================================
AstParam* parParam(Toks* toks) {
  char* nam = parMust(toks, 1, TOKNAM)->lexeme;       // eg: msg
  parMust(toks, 1, TOKCOLON);                         // eg: :
  Tok* tok = parMust(toks, 2, TOKNUM, TOKSTR);        // eg: TOKSTR
  TYP typ = TOKtoTYP(tok);                            // eg: TYPSTR
  return astNewParam(nam, typ);                       // eg: (msg, TYPSTR)
}


// Param => Param*
// eg: count:num
// eg: i:num msg:str top:num
// ============================================================================
AstParam* parParams(Toks* toks) {
  AstParam* params = parParam(toks);
  Tok* tok = toksCurr(toks);
  while (tok->kind == TOKNAM) {
    AstParam* param = parParam(toks);
    parAppend((Ast*) params, (Ast*) param);
    tok = toksCurr(toks);
  }
  return params;
}


// Prog => Fun+
// ============================================================================
AstProg* parProg(Toks* toks) {
  AstFun* fun = parFun(toks);                 // first function
  AstProg* prog = astNewProg(fun);
  while (toks->tokNum <= toks->hiTokNum) {
    AstFun* funNext = parFun(toks);           // next function
    parAppend((Ast*) fun, (Ast*) funNext);    // append onto funs chain
    fun = funNext;                            // move along chain
  }
  return prog;
}


// Reporter function for debugging
// ============================================================================
void parRep(Toks* toks, char* s) {
  printf("%s", s);
  Tok* tok = toksCurr(toks);
  printf("%s %s \n", tokStr(tok->kind), tok->lexeme);
}


// Ret => "ret" Exp
// ============================================================================
AstRet* parRet(Toks* toks) {
  parMust(toks, 1, TOKRET);
  AstExp* exp = parExp(toks);
  return astNewRet(exp);
}


// Say => "say" Exp
// ============================================================================
AstSay* parSay(Toks* toks) {
  parMust(toks, 1, TOKSAY);
  AstExp* exp = parExp(toks);
  AstSay* stm = astNewSay(exp);
  return stm;
}


// Stm => If | Let | Nl | Ret | Say | Stop | While
// ============================================================================
Ast* parStm(Toks* toks) {
  Tok* tok = toksCurr(toks);
  TokKind k = tok->kind;
  if (k == TOKLET) {
    return (Ast*) parLet(toks);
  } else if (k == TOKIF) {
    return (Ast*) parIf(toks);
  } else if (k == TOKNL) {
    return (Ast*) parNl(toks);
  } else if (k == TOKWHILE) {
    return (Ast*) parWhile(toks);
  } else if (k == TOKRET) {
    return (Ast*) parRet(toks);
  } else if (k == TOKSAY) {
    return (Ast*) parSay(toks);
  } else if (k == TOKSTOP) {
    return (Ast*) parStop(toks);
  } else {
    utDieStrTokStr("parStm", tok, "a statement");
  }
  return 0;
}


// Test whether the Tok kind 'k' is any of those that can legitimately follow
// an Stm.  This is used to determine when we have finished parsing arguments
// to a function call.  If 'k' can be such a follower, return 1, else 0
// ============================================================================
int parStmFollower(TokKind k) {
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
Ast* parStms(Toks* toks) {
  Ast* stms = parStm(toks);

  TokKind k = toksCurr(toks)->kind;
  while (k != TOKNUF && k != TOKFI && k != TOKELIHW) {
    Ast* stm = parStm(toks);
    parAppend(stms, stm);
    if (toksAtEnd(toks)) break;
    Tok* tok = toksCurr(toks);
    k = tok->kind;
  }
  return stms;
}


// Stop => "stop"
// ============================================================================
AstStop* parStop(Toks* toks) {
  parMust(toks, 1, TOKSTOP);
  return astNewStop();
}


// ============================================================================
AstStr* parStr(Toks* toks) {
  Tok* tok = parMust(toks, 1, TOKSTR);
  return astNewStr(tok->lexeme);
}


// Var => "var" Nam ":" Type
// eg: var count:num
// ============================================================================
AstVar* parVar(Toks* toks) {
  parMust(toks, 1, TOKVAR);                         // eg: var
  Tok* tokNam = parMust(toks, 1, TOKNAM);           // eg: count
  parMust(toks, 1, TOKCOLON);                       // eg: :
  Tok* tokType = parMust(toks, 2, TOKNUM, TOKSTR);  // eg: num
  TYP typ = TOKtoTYP(tokType);                      // eg: TYPNUM
  return astNewVar(tokNam->lexeme, typ);            // eg: (count, TYPNUM)
}


// Vars => Var*
// ============================================================================
AstVar* parVars(Toks* toks) {
  Tok* tok = toksCurr(toks);
  if (tok->kind != TOKVAR) return NULL;

  AstVar* vars = parVar(toks);          // first "var"
  tok = toksCurr(toks);
  while (tok->kind == TOKVAR) {         // subsequent "var"s
    AstVar* var = parVar(toks);
    parAppend((Ast*) vars, (Ast*) var);
    tok = toksCurr(toks);
  }
  return vars;
}


// While => "while" Exp "do" Stm+ "elihw"
// eg: while n < 10 do let n = n + 1 elihw
// ============================================================================
/*
typedef struct {
  AST     kind;
  Ast*    next;
  AstExp* exp;
  Ast*    stms;
} AstWhile;
*/
AstWhile* parWhile(Toks* toks) {
  parMust(toks, 1, TOKWHILE); 
  AstExp* exp = parExp(toks); 
  parMust(toks, 1, TOKDO); 
  Ast* stms = parStms(toks); 
  parMust(toks, 1, TOKELIHW); 
  AstWhile* whileNode = astNewWhile(exp, stms); 
  //++ Use the grammar above to write the body of parWhile.
  //++ Also, replace the line commented FIXUP

  return whileNode;        // FIXUP
}


// Convert TOK* to corresponding BOP*
// ============================================================================
BOP TOKtoBOP(TokKind k) {
  switch(k) {
    case TOKADD: return BOPADD;
    case TOKSUB: return BOPSUB;
    case TOKMUL: return BOPMUL;
    case TOKDIV: return BOPDIV;
    case TOKLE:  return BOPLE;
    case TOKLT:  return BOPLT;
    case TOKNE:  return BOPNE;
    case TOKEEQ: return BOPEEQ;
    case TOKGE:  return BOPGE;
    case TOKGT:  return BOPGT;
    default:     utDie2Str("TOKtoBOP", "Invalid input token");
  }
  return BOPBAD;               // pacify the compiler
}


// Convert Tog language types to corresponding TYP* member
// ============================================================================
TYP TOKtoTYP(Tok* tok) {
  char* nam = tok->lexeme;
  if (strcmp(nam, "fun")  == 0) return TYPFUN;
  if (strcmp(nam, "none") == 0) return TYPNONE;
  if (strcmp(nam, "num")  == 0) return TYPNUM;
  if (strcmp(nam, "str")  == 0) return TYPSTR;
  return TYPUNK;
}

