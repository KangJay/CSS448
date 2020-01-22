// tok.c - functions to handle tokens - Jim Hogg, 2019

#include "tok.h"

Tok* tokNew(int kind, char* lexeme, long numlit, int linNum, int colNum) {
  Tok* tok = (Tok*) malloc(sizeof(Tok));
  tok->kind   = kind;
  tok->lexeme = lexeme;
  tok->numlit = numlit;
  tok->linNum = linNum;
  tok->colNum = colNum;
  return tok;
}

char* tokStr(TokKind kind) {
  switch(kind) {
    case TOKADD:      return "TOKADD";
    case TOKBAD:      return "TOKBAD";
    case TOKCALL:     return "TOKCALL";
    case TOKCOLON:    return "TOKCOLON";
    case TOKCOM:      return "TOKCOM";
    case TOKDIV:      return "TOKDIV";
    case TOKDO:       return "TOKDO";
    case TOKELIHW:    return "TOKELIHW";
    case TOKEEQ:      return "TOKEEQ";
    case TOKEQ:       return "TOKEQ";
    case TOKFI:       return "TOKFI";
    case TOKFUN:      return "TOKFUN";
    case TOKGE:       return "TOKGE";
    case TOKGT:       return "TOKGT";
    case TOKNAM:      return "TOKNAM";
    case TOKNL:       return "TOKNL";
    case TOKIF:       return "TOKIF";
    case TOKLE:       return "TOKLE";
    case TOKLET:      return "TOKLET";
    case TOKLT:       return "TOKLT";
    case TOKMUL:      return "TOKMUL";
    case TOKNE:       return "TOKNE";
    case TOKNUF:      return "TOKNUF";
    case TOKNUM:      return "TOKNUM";
    case TOKNUMLIT:   return "TOKNUMLIT";
    case TOKRET:      return "TOKRET";
    case TOKSAY:      return "TOKSAY";
    case TOKSTOP:     return "TOKSTOP";
    case TOKSTR:      return "TOKSTR";
    case TOKSTRLIT:   return "TOKSTRLIT";
    case TOKSUB:      return "TOKSUB";
    case TOKTHEN:     return "TOKTHEN";
    case TOKVAR:      return "TOKVAR";
    case TOKWHILE:    return "TOKWHILE";
    default:          return "TOKBAD";
  }
}
