// toks.c - container of Tokens - Jim Hogg, 2019

#include <stddef.h>       // offsetof
#include <stdlib.h>       // malloc
#include "toks.h"

void toksAdd(Toks* toks, Tok* tok) {
  if (toks->tokNum < MAXTOKNUM) {
    ++toks->tokNum;
    ++toks->hiTokNum;
    toks->tok[toks->tokNum] = *tok;
  } else {
    utDie2Str("toksAdd", "Too many tokens!");
  }
}


// Check whether we are "at the end" of the Toks array.  That's to say, we
// have already processed all the Toks
// ============================================================================
int toksAtEnd(Toks* toks) {
  return toks->tokNum > toks->hiTokNum;
}


// Return the current Tok (ie, the one at the toks->tokNum 'cursor')
// ============================================================================
Tok* toksCurr(Toks* toks) {
  if (toksAtEnd(toks)) {
    utDie2Str("toksCur", "Attempt to access a Tok beyond end of Toks array");
  } else {
    return &toks->tok[toks->tokNum];
  }
  return NULL;            // unreachable; pacify compiler
}


void toksDump(Toks* toks) {
  FILE* f = fopen("TokenDump.txt", "w");
  for (int t = 0; t <= toks->hiTokNum; ++t) {
    Tok* tok = &toks->tok[t];
    fprintf(f,"[%3d] %3d  %10s %10s  %ld (%d, %d) \n",
      t, tok->kind, tokStr(tok->kind), tok->lexeme, tok->numlit,
      tok->linNum, tok->colNum);
  }
  fclose(f);
}


// Move the toks->tokNum 'cursor' forward one step.  But do not access the
// entry: at the end of the program, the cursor will point just beyond the end
// of the Toks array (ie, at entry toks->hiTokNum + 1)
// ============================================================================
void toksNext(Toks* toks) {
  ++toks->tokNum;
}


// Return the previous Tok (ie, the one just before the toks->tokNum 'cursor')
// ============================================================================
Tok* toksPrev(Toks* toks) {
  --toks->tokNum;
  return toksCurr(toks);
}


Toks* toksNew() {
  Toks* toks = malloc(sizeof(Toks));
  toks->tokNum = toks->hiTokNum = -1;
  return toks;
}

void toksRewind(Toks* toks) { toks->tokNum = 0; }
