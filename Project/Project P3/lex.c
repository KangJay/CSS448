// lex.c - Lexical Analyzer for the Tog Interpreter - Jim Hogg 2019

#include "lex.h"

// Create a new Lex object
// ============================================================================
Lex* lexNew(char* text) {
  Lex* lex = malloc(sizeof(Lex));
  lex->text = text;
  lex->pos = 0;
  lex->linNum = lex->colNum = 1;
  return lex;
}

// Return the current char in lex->text, located at position lex->pos.
// This is a "peek" function that does not change the cursor (ie, lex->pos).
// We name it as lexPeek0, because we are looking at offset 0 from lex->pos
// ============================================================================
char lexPeek0(Lex* lex) { return lex->text[lex->pos]; }


// Return the next char in lex->text: the one located at position
// lex->pos + 1.  This is a "peek" function that does not change the cursor
// (ie, lex->pos).  We name it as lexPeek1, because we are looking at offset
// 1 from lex->pos
// ============================================================================
char lexPeek1(Lex* lex) { return lex->text[lex->pos + 1]; }


// Move the cursor (lex->pos) forward by 1.  Return the char it then points at.
// ============================================================================
char lexMove1(Lex* lex) {
  ++lex->colNum;
  ++lex->pos;
  return lex->text[lex->pos];
}


// Skip over whitespace: any ASCII control char from 0x01 (SOH) thru 0x1F (US),
// as well as 0x20 (space).  This includes all those chars normally described
// as "whitespace", such as tab and carriage-return.  If the char is a newline,
// we additionally bump Lex's line number, and reset the column number
// ============================================================================
char lexSkip(Lex* lex) {
  char c = lexPeek0(lex);

  while (c >= 0x01 && c <= 0x20) {
    if (c == '\n') {                // 0x0A
      ++lex->linNum;
      lex->colNum = 1;
    }
    c = lexMove1(lex);
  }
  return c;
}


// Scan a string.  On entry, lex->pos points at the opening ' char.
// Eg: 'hello' will create a Token of kind TOKSTR, stripping out
// the leading and trailing tick (')
// ============================================================================
Tok* lexStr(Lex* lex) {
  char c = lexMove1(lex);                             // eg: h
  int start = lex->pos;                               // eg: 60 => h
  while (c != '\'') c = lexMove1(lex);                // scan to trailing '
  int len = lex->pos - start;                         // eg: 65 - 60 = 5
  char* str = utStrndup(&lex->text[start], len);      // eg: hello
  c = lexMove1(lex);                                  // skip closing '
  return tokNew(TOKSTRLIT, str, 0, lex->linNum, lex->colNum);
}

// Scan a comment (starting with #).  Eg: lex->text = "++x; # bump x " with
// lex->pos = 5, will return "# bump x ", leaving lex->pos = 14.  On entry,
// lex->pos points at #

Tok* lexComment(Lex* lex) {
  int start = lex->pos;                               // eg: 5 => '#'
  int c = lexPeek0(lex);                              // eg: '#'
  while (c != '\n') c = lexMove1(lex);
  int len = lex->pos - start;                         // eg: 14
  char* str = utStrndup(&lex->text[start], len);
  return tokNew(TOKCOM, str, 0, lex->linNum, lex->colNum);
}


// Extract the number (a string of digits), starting at lex->text[lex->pos].
// Eg: lex->text = "x = 1234; ", lex->pos = 4, will return 1234, and leave
// lex->pos = 8.  (Note: on entry, lex->pos points to the first digit in
// the number)
// ============================================================================
Tok* lexNum(Lex* lex) {
  int start = lex->pos;
  long sum = lexPeek0(lex) - '0';                     // eg: 1
  char c = lexMove1(lex);
  while (isdigit(c)) {
    sum = 10 * sum + (c - '0');                       // eg: 10 * 1 + 2
    c = lexMove1(lex);
  }

  int len = lex->pos - start;
  char* lexeme = utStrndup(&lex->text[start], len);
  Tok* tok = tokNew(TOKNUMLIT, lexeme, sum, lex->linNum, lex->colNum);
  return tok;
}


// Scan punctuation
// ============================================================================
Tok* lexPun(Lex* lex) {
  char c0 = lexPeek0(lex);
  char c1 = lexPeek1(lex);

  // First check for two-letter tokens

  if (c0 == '<' && c1 == '=') { lex->pos += 2; return tokNew(TOKLE,  "<=", 0, lex->linNum, lex->colNum); }
  if (c0 == '=' && c1 == '=') { lex->pos += 2; return tokNew(TOKEEQ, "==", 0, lex->linNum, lex->colNum); }
  if (c0 == '!' && c1 == '=') { lex->pos += 2; return tokNew(TOKNE,  "!=", 0, lex->linNum, lex->colNum); }
  if (c0 == '>' && c1 == '=') { lex->pos += 2; return tokNew(TOKGE,  ">=", 0, lex->linNum, lex->colNum); }

  // Next, check for single-letter tokens

  if (c0 == '+')  { ++lex->pos;   return tokNew(TOKADD,    "+",  0, lex->linNum, lex->colNum); }
  if (c0 == '-')  { ++lex->pos;   return tokNew(TOKSUB,    "-",  0, lex->linNum, lex->colNum); }
  if (c0 == '*')  { ++lex->pos;   return tokNew(TOKMUL,    "*",  0, lex->linNum, lex->colNum); }
  if (c0 == '/')  { ++lex->pos;   return tokNew(TOKDIV,    "/",  0, lex->linNum, lex->colNum); }
  if (c0 == ':')  { ++lex->pos;   return tokNew(TOKCOLON,  ":",  0, lex->linNum, lex->colNum); }
  if (c0 == '=')  { ++lex->pos;   return tokNew(TOKEQ,     "=",  0, lex->linNum, lex->colNum); }
  if (c0 == '<')  { ++lex->pos;   return tokNew(TOKLT,     "<",  0, lex->linNum, lex->colNum); }
  if (c0 == '>')  { ++lex->pos;   return tokNew(TOKGT,     ">",  0, lex->linNum, lex->colNum); }

  utDie2StrCharLC("lexPun", "unrecognized punctuation. c = ", c0, lex->linNum, lex->colNum);

  return NULL;    // pacify the compiler

}


// Extract the name (a string of alphanumeric chars), starting at
// lex->text[lex->pos].  Eg: lex->text = " TotalSum = ", lex->pos = 1, will
// return "TotalSum", and leave lex->pos = 9.  On entry, lex->pos is pointing
// at the first (alphabetic char of the identifier's lexeme (eg: 'T').  On
// exit, lex->pos is pointing at the char that is NOT part of the name
// ============================================================================
Tok* lexNam(Lex* lex) {
  int start = lex->pos;                               // eg: 1
  char c = lexMove1(lex);
  while (isalnum(c)) c = lexMove1(lex);
  int len = lex->pos - start;                         // eg: 8
  char* nam = utStrndup(&lex->text[start], len);
  return tokNew(TOKNAM, nam, 0, lex->linNum, lex->colNum);
}


// Extract all tokens in lex->text, starting at position lex->pos
// (invariably 0).  As each token is constructed, insert it into the 'toks'
// array
// ============================================================================
Toks* lexGo(Lex* lex) {
  Toks* toks = toksNew();

  char c = lexSkip(lex);
  Tok* tok;

  while (c) {                     // scan every char
    if (isdigit(c)) {             // [0-9]
      tok = lexNum(lex);
    } else if (isalpha(c)) {      // [a-zA-Z]
      tok = lexNam(lex);
      lexKeyword(&tok);           // check if keyword (if, then, while, etc)
    } else if (c == '#') {
      tok = lexComment(lex);
    } else if (c == '\'') {
      tok = lexStr(lex);
    } else {
      tok = lexPun(lex);          // # ( ) = < <= == >= > + - * / '
    }
    if (tok->kind != TOKCOM) toksAdd(toks, tok);  // discard comments

    c = lexSkip(lex);
  }
  return toks;
}


// Check whether the current Tok (of kind TOKNAM) is any of the keywords in
// the Tog language.  If yes, adjust the Token kind accordingly
// ============================================================================
void lexKeyword(Tok** tok) {
  Tok* p = *tok;                  // alias
  char* s = p->lexeme;
  if (strcmp(s, "call")     == 0) { p->kind = TOKCALL;     return; }
  if (strcmp(s, "do")       == 0) { p->kind = TOKDO;       return; }
  if (strcmp(s, "elihw")    == 0) { p->kind = TOKELIHW;    return; }
  if (strcmp(s, "fi")       == 0) { p->kind = TOKFI;       return; }
  if (strcmp(s, "fun")      == 0) { p->kind = TOKFUN;      return; }
  if (strcmp(s, "if")       == 0) { p->kind = TOKIF;       return; }
  if (strcmp(s, "let")      == 0) { p->kind = TOKLET;      return; }
  if (strcmp(s, "nl")       == 0) { p->kind = TOKNL;       return; }
  if (strcmp(s, "nuf")      == 0) { p->kind = TOKNUF;      return; }
  if (strcmp(s, "num")      == 0) { p->kind = TOKNUM;      return; }
  if (strcmp(s, "ret")      == 0) { p->kind = TOKRET;      return; }
  if (strcmp(s, "say")      == 0) { p->kind = TOKSAY;      return; }
  if (strcmp(s, "stop")     == 0) { p->kind = TOKSTOP;     return; }
  if (strcmp(s, "str")      == 0) { p->kind = TOKSTR;      return; }
  if (strcmp(s, "then")     == 0) { p->kind = TOKTHEN;     return; }
  if (strcmp(s, "var")      == 0) { p->kind = TOKVAR;      return; }
  if (strcmp(s, "while")    == 0) { p->kind = TOKWHILE;    return; }
}



