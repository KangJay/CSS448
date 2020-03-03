// toks.h - collection of Tokens - Jim Hogg, 2019

#pragma once

#include "tok.h"            // Tok
#include "ut.h"             // ut*

typedef struct _Toks {
  #define MAXTOKNUM 999
  int tokNum;               // current Tok number (iterator)
  int hiTokNum;             // hightest Tok number in current Toks object
  Tok tok[MAXTOKNUM + 1];
} Toks;


void  toksAdd(Toks* toks, Tok* tok);
int   toksAtEnd(Toks* toks);
void  toksDump(Toks* toks);
Toks* toksNew();
void  toksRewind(Toks* toks);
Tok*  toksCurr(Toks* toks);
void  toksNext(Toks* toks);
Tok*  toksPrev(Toks* toks);


