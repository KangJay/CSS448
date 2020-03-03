// visit.h - Visit every node of the AST - Jim Hogg, 2019

#pragma once

#include "ast.h"

void visitBop(BOP bop);
void visitBody(AstBody* ast);
void visitCall(AstCall* ast);
void visitExp(AstExp* ast);
void visitFun(AstFun* ast);
void visitFuns(AstFun* ast);
void visitIf(AstIf* ast);
void visitLet(AstLet* ast);
void visitNam(AstNam* ast);
void visitNl(AstNl* ast);
void visitNum(AstNum* ast);
void visitParam(AstParam* ast);
void visitProg(AstProg* ast);
void visitRet(AstRet* ast);
void visitSay(AstSay* ast);
void visitStms(Ast* ast);
void visitStop(AstStop* ast);
void visitStr(AstStr* ast);
void visitVar(AstVar* ast);
void visitVars(AstVar* ast);
void visitWhile(AstWhile* ast);
