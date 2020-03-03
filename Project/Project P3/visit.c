// visit.c - walk a Tog AST, vising every Node - Jim Hogg, 2019

#include "visit.h"

//Visit BOP
void visitBop(BOP bop) {
  printf("Bop ");
  switch(bop){
    case BOPADD:   
      printf("+ ");
      break; 
    case BOPBAD:   
      printf("BOPBAD ");
      break;
    case BOPDIV:   
      printf("/ ");
      break;
    case BOPEEQ:   
      printf("== ");
      break;
    case BOPGE:    
      printf(">= ");
      break;
    case BOPGT:    
      printf("> ");
      break;
    case BOPLE:    
      printf("<= ");
      break;
    case BOPLT:    
      printf("< ");
      break;
    case BOPMUL:   
      printf("* ");
      break;
    case BOPNE:    
      printf("!= ");
      break;
    case BOPSUB:   
      printf("- ");
      break;
    default:       printf("BOPBAD ");;
  }
}

// Body => Var* Stm+
// ==============================================
void visitBody(AstBody* ast) {
  printf("Body ");
  visitVars(ast->vars);
  visitStms(ast->stms);
}


//  Call => "call" Nam "(" NamLit* ")"
// ========================================================
void visitCall(AstCall* ast) {
  printf("Call ");
  printf("%s ", ast->nam);
  Ast* arg = ast->args;
  while (arg != NULL) {
    if (arg->kind == ASTNAM) {
      visitNam((AstNam*) arg);
    } else if (arg->kind == ASTNUM) {
      visitNum((AstNum*) arg);
    } else if (arg->kind == ASTSTR) {
      visitStr((AstStr*) arg);
    }
    arg = arg->next;
  }
}


// Exp => NamLit | NamLit Bop NamLit | Call
// ==============================================
void visitExp(AstExp* ast) {
  printf("Exp ");

  AST lhsKind = ast->lhs->kind;
  if (lhsKind == ASTNUM) {                  // Exp => Num
    visitNum((AstNum*) ast->lhs);
  } else if (lhsKind == ASTNAM) {           // Exp => Nam
    visitNam((AstNam*) ast->lhs);
  } else if (lhsKind == ASTSTR) {           // Exp => Str
    visitStr((AstStr*) ast->lhs);
  } else if (lhsKind == ASTCALL) {          // Exp => Call
    visitCall((AstCall*) ast->lhs);
  }

  if (ast->rhs == NULL) return;

  AST rhsKind = ast->rhs->kind;
  if (ast->bop != BOPNONE) {                 // NamLit Bop NamLit
    visitBop(ast->bop);
    if (rhsKind == ASTNUM) {
      visitNum((AstNum*) ast->rhs);
    } else if (rhsKind == ASTNAM) {
      visitNam((AstNam*) ast->rhs);
    }
  }
}

// Fun => "fun" Nam ":" Type Param+ "=" Body "nuf"
//      | "main" "=" Body "nuf"
// ========================================================
void visitFun(AstFun* ast) {
  printf("Fun ");
  printf("%s ", ast->nam);
  AstParam* param = ast->params;
  while (param != NULL) {
    visitParam(param);
    param = (AstParam*) param->next;
  }
  visitBody(ast->body);
  printf("Nuf ");
}


// Funs => Fun+
// ========================================================
void visitFuns(AstFun* ast) {
  while (ast != NULL) {
    visitFun(ast);
    ast = (AstFun*) ast->next;
  }
}



// If => "if" Exp "then" Stm+ "fi"
// ========================================================
void visitIf(AstIf* ast) {
  printf("If ");
  visitExp(ast->exp);
  visitStms(ast->stms);
  printf("Fi ");
}


// Let => "let" Nam "=" Exp
// ========================================================
void visitLet(AstLet* ast) {
  printf("Let ");
  printf("%s ", ast->nam);
  visitExp(ast->exp);
}


// ========================================================
void visitNum(AstNum* ast) {
  printf("Num ");
  printf("%lg ", ast->num);
}


// ========================================================
void visitParam(AstParam* ast) {
  printf("Param ");
  printf("%s ", ast->nam);
  printf("%s ", astTYPtoStr(ast->typ));
}


// Prog => Fun+
// ========================================================
void visitProg(AstProg* astProg) {
  printf("Prog ");
  AstFun* ast = astProg->funs;
  while (ast != NULL) {
    visitFun(ast);
    ast = (AstFun*) ast->next;
  }
  printf("\n\n");
}


// ========================================================
void visitNam(AstNam* ast) {
  printf("Nam ");
  printf("%s ", ast->nam);
}


// Nl => "nl"
// ========================================================
void visitNl(AstNl* ast) {
  printf("Nl ");
}


// Ret => "ret" Exp
// ========================================================
void visitRet(AstRet* ast) {
  printf("Ret ");
  visitExp(ast->exp);
}

// Say => "say" Exp
// ========================================================
void visitSay(AstSay* ast) {
  printf("Say ");
  visitExp(ast->exp);
}

// ========================================================
void visitStms(Ast* ast) {
  while (ast != NULL) {
    switch(ast->kind) {
      case ASTBODY:  visitBody ((AstBody*) ast);    break;
      case ASTCALL:  visitCall ((AstCall*) ast);    break;
      case ASTEXP:   visitExp  ((AstExp*)  ast);    break;
      case ASTFUN:   visitFun  ((AstFun*)  ast);    break;
      case ASTIF:    visitIf   ((AstIf*)   ast);    break;
      case ASTLET:   visitLet  ((AstLet*)  ast);    break;
      case ASTNAM:   visitNam  ((AstNam*)   ast);   break;
      case ASTNL:    visitNl   ((AstNl*)    ast);   break;
      case ASTNUM:   visitNum  ((AstNum*)   ast);   break;
      case ASTRET:   visitRet  ((AstRet*)   ast);   break;
      case ASTSAY:   visitSay  ((AstSay*)   ast);   break;
      case ASTSTOP:  visitStop ((AstStop*)  ast);   break;
      case ASTSTR:   visitStr  ((AstStr*)   ast);   break;
      case ASTVAR:   visitVar  ((AstVar*)   ast);   break;
      case ASTWHILE: visitWhile((AstWhile*) ast);   break;
      default:                                      break;
    }
    ast = ast->next;
  }
}


// Stop => "stop"
// ========================================================
void visitStop(AstStop* ast) {
  printf("Stop ");
}



// ========================================================
void visitStr(AstStr* ast) {
  printf("Str ");
  printf("'%s' ", ast->str);
}


// ========================================================
void visitVar(AstVar* ast) {
  printf("Var ");
  printf("%s ", ast->nam);
  printf("%s ", astTYPtoStr(ast->typ));
}


// ========================================================
void visitVars(AstVar* ast) {
  if (ast == NULL) return;      // function has no vars
  visitVar(ast);
  ast = (AstVar*) ast->next;
  while (ast != NULL) {
    visitVar((AstVar*) ast);
    ast = (AstVar*) ast->next;
  }
}


// While => "while" Exp "do" Stm+ "elihw"
// ========================================================
void visitWhile(AstWhile* ast) {
  printf("While ");
  visitExp(ast->exp);
  visitStms(ast->stms);
}

