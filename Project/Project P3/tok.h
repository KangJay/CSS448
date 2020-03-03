// tok.h - Tokens - Jim Hogg, 2019

#pragma once

#include <stdio.h>      // printf
#include <stdlib.h>     // malloc
#include <string.h>     // strlen

typedef enum {
  TOKADD = 1, TOKBAD, TOKCALL, TOKCOLON, TOKCOM, TOKDIV, TOKDO, TOKELIHW,
  TOKEEQ, TOKEQ, TOKFI, TOKFUN, TOKGE, TOKGT, TOKIF, TOKLE, TOKLET, TOKLT,
  TOKMUL, TOKNL, TOKNAM, TOKNE, TOKNUF, TOKNUM, TOKNUMLIT, TOKRET, TOKSAY,
  TOKSTOP, TOKSTR, TOKSTRLIT, TOKSUB, TOKTHEN, TOKVAR, TOKWHILE
} TokKind;

char* tokStr(TokKind kind);

typedef struct _Tok {
  TokKind kind;       // eg: TOKNUM
  char*   lexeme;     // eg: "123" for TOKNUMLIT; "# comment" for TOKCOM; "hello" for TOKSTRLIT
  long    numlit;     // eg: 123 for TOKNUMLIT
  int     linNum;     // eg: 14
  int     colNum;     // eg: 8
} Tok;

Tok* tokNew(int kind, char* lexeme, long numlit, int linNum, int colNum);

