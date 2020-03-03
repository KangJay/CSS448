// rex.h - Recognizer - Jim Hogg, 2019

#pragma once

#include <stdarg.h>

#include "toks.h"       // Toks

void rexBody(Toks* toks);
void rexCall(Toks* toks);
void rexExp(Toks* toks);
void rexFun(Toks* toks);
void rexIf(Toks* toks);
int  rexIsBop(TokKind k);
void rexLet(Toks* toks);
void rexMust(Toks* toks, TokKind k1);
void rexMust2(Toks* toks, TokKind k1, TokKind k2);
void rexMust3(Toks* toks, TokKind k1, TokKind k2, TokKind k3);
void rexMust4(Toks* toks, TokKind k1, TokKind k2, TokKind k3, TokKind k4);
void rexNamLit(Toks* toks);
void rexNumNam(Toks* toks);
void rexParams(Toks* toks);
void rexProg(Toks* toks);
void rexRet(Toks* toks);
void rexSay(Toks* toks);
int  rexStmFollower(TokKind k);
void rexStm(Toks* toks);
void rexStms(Toks* toks);
void rexStop(Toks* toks);
void rexStr(Toks* toks);
void rexVar(Toks* toks);
void rexVars(Toks* toks);
void rexWhile(Toks* toks);

