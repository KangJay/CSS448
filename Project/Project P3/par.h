// par.h - Parser - Jim Hogg, 2019

#pragma once

#include <stdarg.h>

#include "ast.h"        // AstBody, etc
#include "toks.h"       // Toks

AstBody*   parBody   (Toks* toks);
AstCall*   parCall   (Toks* toks);
AstExp*    parExp    (Toks* toks);
AstFun*    parFun    (Toks* toks);
AstIf*     parIf     (Toks* toks);
int        parIsBop  (TokKind k);
AstLet*    parLet    (Toks* toks);
Ast*       parNamLit (Toks* toks);
Ast*       parNumNam (Toks* toks);
AstParam*  parParams (Toks* toks);
AstProg*   parProg   (Toks* toks);
AstRet*    parRet    (Toks* toks);
AstSay*    parSay    (Toks* toks);
int        parStmFollower(TokKind k);
Ast*       parStm    (Toks* toks);
Ast*       parStms   (Toks* toks);
AstStop*   parStop   (Toks* toks);
AstStr*    parStr    (Toks* toks);
AstVar*    parVar    (Toks* toks);
AstVar*    parVars   (Toks* toks);
AstWhile*  parWhile  (Toks* toks);

Tok* parMust(Toks* toks, int numk, ...);

void parAppend(Ast* as, Ast* a);

BOP TOKtoBOP(TokKind k);
TYP   TOKtoTYP(Tok* tok);

