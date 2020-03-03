// ast.h - Abstract Syntax Trees - Jim Hogg, 2019

#pragma once

#include <assert.h>         // assert
#include <ctype.h>          // isspace, isalpha, isdigit, isalnum
#include <stdio.h>          // printf
#include <stdlib.h>         // calloc
#include <string.h>         // strncpy

#include "tok.h"            // TokKind
#include "toks.h"           // Toks
#include "ut.h"             // ut*

typedef enum {
  TYPAST = 1, TYPFRAME, TYPFUN, TYPNONE, TYPNUM, TYPSTR, TYPUNK
} TYP;
char* astTYPtoStr(TYP typ);

typedef enum {                                          // Binary Operators
  BOPADD = 1, BOPSUB, BOPMUL, BOPDIV, BOPNONE, BOPBAD,  // Arithmetic
  BOPLT, BOPLE, BOPNE, BOPEEQ, BOPGE, BOPGT             // Relational
} BOP;
char* astBOPtoStr(BOP bop);

typedef enum {
  ASTBODY = 1, ASTCALL, ASTEXP, ASTFUN, ASTIF, ASTLET, ASTNAM, ASTNL, ASTNUM,
  ASTPARAM, ASTPROG, ASTRET, ASTSAY, ASTSTOP, ASTSTR, ASTVAR, ASTWHILE
} AST;


// Common 'super' struct for all ASTs
// ============================================================================
typedef struct Ast_ {
  AST    kind;
  struct Ast_* next;
} Ast;


struct AstVar_;     typedef struct AstVar_ AstVar;      // forward declaration
struct AstParam_;   typedef struct AstParam_ AstParam;  // forward declaration


// Body => Var* Stm+
// ============================================================================
typedef struct {
  AST     kind;
  Ast*    next;
  AstVar* vars;
  Ast*    stms;
} AstBody;
AstBody* astNewBody(AstVar* vars, Ast* stms);


// Call => "call" Nam NamLit*
typedef struct {
  AST   kind;
  Ast*  next;
  char* nam;
  Ast*  args;
} AstCall;
AstCall* astNewCall(char* nam, Ast* args);


// Exp => NamLit | NamLit Bop NamLit | Call
// ============================================================================
typedef struct {
  AST  kind;
  Ast* next;
  Ast* lhs;
  BOP  bop;
  Ast* rhs;
} AstExp;
AstExp* astNewExp(Ast* lhs, BOP bop, Ast* rhs);


// Fun => "fun" Nam ":" Type Param+ "=" Body "nuf"
// eg: fun addone:num n:num = var np1 let np1 = n + 1 ret np1 nuf
// ============================================================================
typedef struct {
  AST       kind;           // eg: ASTFUN
  Ast*      next;           // next Fun in Prog
  char*     nam;            // eg: addone
  TYP       typ;            // eg: num
  AstParam* params;         // eg: n:num
  AstBody*  body;           // eg: var np1 let np1 = n + 1 ret np1
} AstFun;
AstFun* astNewFun(char* nam, TYP typ, AstParam* params, AstBody* body);

// If => "if" Exp "then" Stm+ "fi"
// ============================================================================
typedef struct {
  AST     kind;
  Ast*    next;
  AstExp* exp;
  Ast*    stms;
} AstIf;
AstIf* astNewIf(AstExp* exp, Ast* stms);


// Let => "let" Nam "=" Exp
// ============================================================================
typedef struct {
  AST     kind;             // ASTLET
  Ast*    next;
  char*   nam;              // eg: abc
  AstExp* exp;              // eg: x + 1
} AstLet;
AstLet* astNewLet(char* nam, AstExp* exp);


// An AST node that represent a name.  Typically, this is a reference to some
// variable, such as "counter" in the body of a function.  At that point, we
// do not know its Type.
// ============================================================================
typedef struct {
  AST   kind;
  Ast*  next;
  char* nam;
} AstNam;
AstNam* astNewNam(char* nam);


// Nl => "nl"
// ============================================================================
typedef struct {
  AST  kind;
  Ast* next;
} AstNl;
AstNl* astNewNl();


// An AST node that represent a simple, literal number.  Eg: 42
// ============================================================================
typedef struct {
  AST    kind;
  Ast*   next;
  double num;
} AstNum;
AstNum* astNewNum(double num);


// Param => Nam ":" Type | ABSENT
// ============================================================================
typedef struct AstParam_ {
  AST     kind;             // ASTPARAM
  Ast*    next;
  char*   nam;              // eg: count        greeting
  TYP     typ;              // eg: TYPNUM       TYPSTR
} AstParam;
AstParam* astNewParam(char* nam, TYP typ);

// Prog => Fun+
// ============================================================================
typedef struct {
  AST     kind;
  Ast*    next;
  AstFun* funs;
} AstProg;
AstProg* astNewProg(AstFun* funs);


// Ret => "ret" Exp
// ============================================================================
typedef struct {
  AST     kind;
  Ast*    next;
  AstExp* exp;
} AstRet;
AstRet* astNewRet(AstExp* exp);


// Say => "say" Exp
// ============================================================================
typedef struct {
  AST     kind;
  Ast*    next;
  AstExp* exp;
} AstSay;
AstSay* astNewSay(AstExp* exp);


// Stop => "stop"
// ============================================================================
typedef struct {
  AST  kind;
  Ast* next;
} AstStop;
AstStop* astNewStop();


// An AST node that represent a simple, literal string.  Eg: "hello world"
// ============================================================================
typedef struct {
  AST   kind;
  Ast*  next;
  char* str;
} AstStr;
AstStr* astNewStr(char* str);


// Var => "var" Nam ":" Type
// ============================================================================
typedef struct AstVar_ {
  AST     kind;             // ASTVAR
  Ast*    next;
  char*   nam;              // eg: count        greeting
  TYP     typ;              // eg: TYPNUM       TYPSTR
} AstVar;
AstVar* astNewVar(char* nam, TYP typ);


// While => "while" Cmp "do" Stm+ "elihw"
// ============================================================================
typedef struct {
  AST     kind;
  Ast*    next;
  AstExp* exp;
  Ast*    stms;
} AstWhile;
AstWhile* astNewWhile(AstExp* exp, Ast* stms);


AstFun* astFindFun(AstProg* astProg, char* nam);

