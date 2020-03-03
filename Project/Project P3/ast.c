// ast.c - Abstract Syntax Tree - Jim Hogg, 2019

#include "ast.h"

// Search the program AST rooted at 'astProg' looking for the function
// called 'nam'.  If not found, return NULL
// ============================================================================
AstFun* astFindFun(AstProg* astProg, char* nam) {
  assert(astProg->kind == ASTPROG);
  AstFun* astFun = astProg->funs;
  while (astFun) {
    if (strcmp(astFun->nam, nam) == 0) return astFun;
    astFun = (AstFun*) astFun->next;
  }
  return NULL;
}


AstBody* astNewBody(AstVar* vars, Ast* stms) {
  AstBody* a = calloc(sizeof(AstBody), 1);
  a->kind = ASTBODY; a->vars = vars; a->stms = stms;
  return a;
}


AstCall* astNewCall(char* nam, Ast* args) {
  AstCall* a = calloc(sizeof(AstCall), 1);
  a->kind = ASTCALL; a->nam = nam; a->args = args;
  return a;
}


AstExp* astNewExp(Ast* lhs, BOP bop, Ast* rhs) {
  AstExp* a = calloc(sizeof(AstExp), 1);
  a->kind = ASTEXP; a->lhs = lhs; a->bop = bop; a->rhs = rhs;
  return a;
}


AstFun* astNewFun(char* nam, TYP typ, AstParam* params, AstBody* body) {
  AstFun* a = calloc(sizeof(AstFun), 1);
  a->kind = ASTFUN; a->nam = nam; a->typ = typ; a->params = params; a->body = body;
  return a;
}


AstIf* astNewIf(AstExp* exp, Ast* stms) {
  AstIf* a = calloc(sizeof(AstIf), 1);
  a->kind = ASTIF; a->exp = exp; a->stms = stms;
  return a;
}


AstLet* astNewLet(char* nam, AstExp* exp) {
  AstLet* a = calloc(sizeof(AstLet), 1);
  a->kind = ASTLET; a->nam = nam; a->exp = exp;
  return a;
}


AstNam* astNewNam(char* nam) {
  AstNam* a = calloc(sizeof(AstNam), 1);
  a->kind = ASTNAM; a->nam = nam;
  return a;
}


AstNl* astNewNl() {
  AstNl* a = calloc(sizeof(AstNl), 1);
  a->kind = ASTNL;
  return a;
}


AstNum* astNewNum(double num) {
  AstNum* a = calloc(sizeof(AstNum), 1);
  a->kind = ASTNUM; a->num = num;
  return a;
}


AstParam* astNewParam(char* nam, TYP typ) {
  AstParam* a = calloc(sizeof(AstParam), 1);
  a->kind = ASTPARAM; a->next = 0; a->nam = nam; a->typ = typ;
  return a;
}


AstProg* astNewProg(AstFun* funs) {
  AstProg* a = calloc(sizeof(AstProg), 1);
  a->kind = ASTPROG; a->funs = funs;
  return a;
}


AstRet* astNewRet(AstExp* exp) {
  AstRet* a = calloc(sizeof(AstRet), 1);
  a->kind = ASTRET; a->exp = exp;
  return a;
}


AstSay* astNewSay(AstExp* exp) {
  AstSay* a = calloc(sizeof(AstSay), 1);
  a->kind = ASTSAY; a->exp = exp;
  return a;
}

AstStop* astNewStop() {
  AstStop* a = calloc(sizeof(AstStop), 1);
  a->kind = ASTSTOP;
  return a;
}

AstStr* astNewStr(char* str) {
  AstStr* a = calloc(sizeof(AstStr), 1);
  a->kind = ASTSTR; a->str = str;
  return a;
}


AstVar* astNewVar(char* nam, TYP typ) {
  AstVar* a = calloc(sizeof(AstVar), 1);
  a->kind = ASTVAR; a->next = 0; a->nam = nam; a->typ = typ;
  return a;
}


AstWhile* astNewWhile(AstExp* exp, Ast* stms) {
  AstWhile* a = calloc(sizeof(AstWhile), 1);
  a->kind = ASTWHILE; a->exp = exp; a->stms = stms;
  return a;
}


char* astBOPtoStr(BOP bop) {
  switch(bop) {
    case BOPADD:   return "BOPADD";
    case BOPBAD:   return "BOPBAD";
    case BOPDIV:   return "BOPDIV";
    case BOPEEQ:   return "BOPEEQ";
    case BOPGE:    return "BOPGE";
    case BOPGT:    return "BOPGT";
    case BOPLE:    return "BOPLE";
    case BOPLT:    return "BOPLT";
    case BOPMUL:   return "BOPMUL";
    case BOPNE:    return "BOPNE";
    case BOPNONE:  return "BOPNONE";
    case BOPSUB:   return "BOPSUB";
    default:       return "BOPBAD";
  }
}


char* astTYPtoStr(TYP typ) {
  switch(typ) {
    case TYPFRAME: return "TYPFRAME";
    case TYPFUN:   return "TYPFUN";
    case TYPNONE:  return "TYPNONE";
    case TYPNUM:   return "TYPNUM";
    case TYPSTR:   return "TYPSTR";
    case TYPUNK:   return "TYPUNK";
    default:       return "TYPUNK";
  }
}
