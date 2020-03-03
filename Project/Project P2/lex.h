// lex.h - Lexical Analyzer - Jim Hogg, 2019

#pragma once

#include <ctype.h>      // isdigit
#include <limits.h>     // INT_MAX
#include <stdio.h>      // printf
#include <stdlib.h>     // exit

#define __STDC_WANT_LIB_EXT2__ 1    // get strdup, strndup
#include <string.h>     // strncpy

#include "tok.h"        // Tok
#include "toks.h"       // Toks
#include "ut.h"         // ut*

typedef struct {
  char* text;         // entire program text to be scanned
  int   pos;          // current char offset into 'text'
  int   linNum;       // current line number (starts at 1)
  int   colNum;       // current column number (starts at 1)
} Lex;

Tok*  lexComment(Lex* lex);
Toks* lexGo(Lex* lex);
void  lexKeyword(Tok** tok);
char  lexMove1(Lex* lex);
Tok*  lexNam(Lex* lex);
Lex*  lexNew(char* text);
char  lexPeek0(Lex* lex);
char  lexPeek1(Lex* lex);
Tok*  lexPun(Lex* lex);
char  lexSkip(Lex* lex);
Tok*  lexStr(Lex* lex);

