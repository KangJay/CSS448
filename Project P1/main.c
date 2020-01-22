// main.c - Compiler for the Tog compiler - Jim Hogg, 2019
//
// Prog       => Fun+
// Fun        => "fun" Nam ":" Type Param+ "=" Body "nuf" | "main" "=" Body "nuf"
// Type       => "num" | "str"
// Param      => Nam ":" Type
// Body       => Var* Stm+
// Var        => "var" Nam ":" Type
// Stm        => If | Let | Nl | Ret | Say | Stop | While
// If         => "if" Exp "then" Stm+ "fi"
// Let        => "let" Nam "=" Exp
// Ret        => "ret" Exp
// Say        => "say" Exp
// Nl         => "nl"
// Stop       => "stop"
// While      => "while" Exp "do" Stm+ "elihw"
// NamLit     => Nam | Num | Str
// Exp        => NamLit | NamLit Bop NamLit | Call
// Bop        => "+" | "-" | "*" | "/" | "<" | "<=" | "!=" | "==" | ">=" | ">"
// Call       => "call" Nam NamLit*
// Num        => [0-9]+
// Str        => "'" NonTicks "'"
// Nam        => Alpha AlphaNum*
// Alpha      => [a-zA-Z]
// AlphaNum   => [a-zA-Z0-9]
// NonTicks   => [^']*
//
//
// Execution starts with the function "main".
// Type 'num' is a C double (8-byte floating-point)
// Type 'str' is a nul-terminated string of ASCII chars (like C)

#include "main.h"

void usage() { printf("\n\nUsage: togc <file.tog> \n\n"); }

int main(int argc, char* argv[]) {
  if (argc < 2) { usage(); exit(-1); }

  char* prog = utReadFile(argv[1]);       // raw chars
  if (!prog){
    printf("File: \'%s\' could not be opened and/or read from.\n", argv[1]); 
    exit(EXIT_FAILURE); 
  }
  //++ Insert a loop that steps thru every char of 'prog' and
  //++ printa its decimal (%d) and character (%c) value
  //++ to the console.

  Lex* lex = lexNew(prog);
  Toks* toks = lexGo(lex);
  toksDump(toks);           // DEBUG: dump Tokens to TokenDump.txt

  utPause();
  return 0;
}
