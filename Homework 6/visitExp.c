#include <stdio.h> 
#include <stdlib.h>
#include <stdbool.h>
#include <string.h> 

/*
Homework 06: Task 3

1. What is a Symbol Table?
    - An auxillary structure that holds a series of 'symbols'. These symbols 
    keep track of the values of variables during interpretation. 
    - It'll keep things such as: The name of the variable, its type, and its numerical
    or string value depending on what kind of variable it is. 
    - To find the value of a variable, you would start at the end of the symbol table
    (the most recent elements added) and traverse backwards until you find the FIRST
    occurence of the variable. 

2. What is a Frame (also known as an Activation Record)?
    - A frame is essentially a delimeter used by the symbol table to denote scope of
    a program during execution. When a function is called, a frame is pushed onto the 
    stack to represent the point in the table before the function call. When the function
    is done executing and the program leaves its scope, it needs to pop the entries
    of the function from the symbol table. It will pop each element until it hits
    this frame. 

3. The table [below] shows a recursive version of a factorial function in Tog.
Draw the Symbol Table at the point where fac has been called recursively, with 
an argument of 0. Show the frames for main, and the multiple instances of fac.

fun fac:num n:num =  
  var ans:num  
  var nm1:num  
  if n == 0 then    
    ret 1  
  fi  
  let nm1 = n - 1  
  let ans = call fac nm1  
  ret ans
nuf

fun main =  
  var f:num  
  let f = call fac 4  
  say f  
  nl
nuf

MAKESHIFT TEXTUAL SYMBOL TABLE:
nam     typ       num     str
------------------------------
main    TYPFUN  
f       TYPNUM
        TYPFRAME
fac     TYPFUN
n       TYPNUM    4
ans     TYPNUM
nm1     TYPNUM    3
        TYPFRAME
fac     TYPFUN
n       TYPNUM    3
ans     TYPNUM
nm1     TYPNUM    2
        TYPFRAME
fac     TYPFUN
n       TYPNUM    2
ans     TYPNUM
nm1     TYPNUM    1
        TYPFRAME
fac     TYPFUN
n       TYPNUM    1
ans     TYPNUM
nm1     TYPNUM    0
      

4. Write the function visitExp(AstExp* ast) in C.  
For each of the following kinds of expression, it should print out:

    - count * 23 ==> "Exp count 23"
    - 42 ==> "Exp 42"
    - hello == 'hello' ==> "Exp hello == 'hello'"
    - call calc numItems ==> "Exp call calc numItems"

To complete this question, you should also write the functions: visitNum, 
visitNam, visitStr and visitCall

Please also define AstStr and AstCall as part of the exercise.  
(No need to write out AstNum or AstNam - we already covered them in lectures)
*/

typedef enum {
  ASTBODY = 1, ASTCALL, ASTEXP, ASTFUN, ASTIF, ASTLET, ASTNAM, ASTNL, ASTNUM,
  ASTPARAM, ASTPROG, ASTRET, ASTSAY, ASTSTOP, ASTSTR, ASTVAR, ASTWHILE
} AST;

typedef enum {  
  BOPADD = 1, BOPSUB, BOPMUL, BOPDIV, BOPNONE, BOPBAD,  // Arithmetic
  BOPLT, BOPLE, BOPNE, BOPEEQ, BOPGE, BOPGT // Relational
} BOP;

typedef enum {  
  TYPAST = 1, TYPFRAME, TYPFUN, TYPNONE, TYPNUM, TYPSTR, TYPUNK
} TYP;

typedef struct Ast_ {  
  AST    kind;
  struct Ast_* next;
} Ast;

typedef struct {
  AST kind; 
  Ast* next;
  Ast* lhs; 
  BOP bop; 
  Ast* rhs; 
} AstExp; 

typedef struct AstParam_ {  
  AST     kind;             // ASTPARAM  
  Ast*    next;  
  char*   nam;              // eg: count        greeting  
  TYP     typ;              // eg: TYPNUM       TYPSTR
} AstParam;


typedef struct {
  AST kind; 
  Ast* next; 
  double num; 
} AstNum; 

typedef struct {
  AST kind; 
  Ast* next; 
  char* nam; 
} AstNam; 

typedef struct {
  AST kind; 
  Ast* next;
  char* nam;  
  TYP typ; 
} AstVar; 

typedef struct {
  AST kind; 
  Ast* next; 
  AstVar* vars; 
  Ast* stms; 
} AstBody; 

typedef struct {
  AST kind; 
  Ast* next;
  char* name; 
  TYP typ; 
  AstParam* params; 
  AstBody* body; 
} AstFun;

//------------------------AstString & AstCall-------------------------------// 
typedef struct {
  AST kind; 
  Ast* next; 
  char* str; 
} AstStr; 

typedef struct {
  AST kind; 
  Ast* next; 
  AstNam* nam;  // name of function
  /* parameters can be name, number, or string so it'll do it there.
     Because of this, I kept it as an Ast pointer and check for its type in visitCall. 
  */
  Ast* namlits;  
} AstCall; 
//-------------------------------------------------------------------------//
void visitNam(AstNam* ast);
void visitNum(AstNum* ast);
void visitStr(AstStr* ast);
void visitCall(AstCall* ast);

/*
typedef struct AstParam_ {  
  AST     kind;             // ASTPARAM  
  Ast*    next;  
  char*   nam;              // eg: count        greeting  
  TYP     typ;              // eg: TYPNUM       TYPSTR
} AstParam;
*/
void visitNamLits(Ast* namlits){
  while (namlits != NULL){
    if (namlits->kind == ASTNAM){
      visitNam((AstNam*) namlits); 
    } else if (namlits->kind == ASTSTR){
      visitStr((AstStr*) namlits); 
    } else if (namlits->kind == ASTNUM){
      visitNum((AstNum*) namlits); 
    }
    namlits = (Ast*) namlits->next; 
  }
}

//Num => [0-9]+
void visitNum(AstNum* ast) {
  printf("%.0f ", ast->num);
}

//Nam => Alpha AlphaNum*
void visitNam(AstNam* ast) {
  printf("%s ", ast->nam);
}

//Str => "NonTicks"
//NonTicks => [^']*
void visitStr(AstStr* ast) {
  printf("\'%s\' ", ast->str);
}

//Call => "call" Nam NamLit*
void visitCall(AstCall* ast) {
  printf("call "); 
  visitNam(ast->nam);
  visitNamLits(ast->namlits); 
}

void printBOP(BOP bop) {
  //BOPNONE shouldn't ever be in here. Was checked beforehand. 
  switch(bop){
    case BOPADD: 
      printf("+ ");
      break; 
    case BOPSUB: 
      printf("- ");
      break; 
    case BOPMUL: 
      printf("* ");
      break; 
    case BOPDIV: 
      printf("/ ");
      break; 
    case BOPBAD: 
      //Not sure what BOPBAD is meant to represent. 
      //I assumed it was an error state. 
      printf("ERROR "); 
      break; 
    case BOPLT: 
      printf("< ");
      break; 
    case BOPLE: 
      printf("<= "); 
      break; 
    case BOPNE: 
      printf("!= "); 
      break; 
    case BOPEEQ: 
      printf("== "); 
      break;  
    case BOPGE: 
      printf(">=");
      break; 
    case BOPGT: 
      printf(">");
      break; 
  }
}

//Exp => NamLit | NamLit Bop NamLit | Call
void visitExp(AstExp* ast) {
  printf("Exp ");
  AST lhsKind = ast->lhs->kind; 
  if (lhsKind == ASTNUM){
    visitNum((AstNum*) ast->lhs); 
  } else if (lhsKind == ASTNAM) {
    visitNam((AstNam*) ast->lhs); 
  } else if (lhsKind == ASTSTR) {
    visitStr((AstStr*) ast->lhs); 
  } else if (lhsKind == ASTCALL) {
    visitCall((AstCall*) ast->lhs); 
  }

  if (ast->rhs == NULL){
    printf("\n");
    return; 
  } 
  //NamLit Bop NamLit case 

  AST rhsKind = ast->rhs->kind; 
  if (ast->bop != BOPNONE) {
    printBOP(ast->bop); 
    if (rhsKind == ASTNUM) {
      visitNum((AstNum*) ast->rhs); 
    } else if (rhsKind == ASTNAM) {
      visitNam((AstNam*) ast->rhs); 
    } else if (rhsKind == ASTSTR){
      visitStr((AstStr*) ast->rhs); 
    }
  }
  printf("\n");
}

//---------------Helper constructor functions to make AstExp nodes------------//
AstExp* constructFirstExample(AST kind, Ast* next, char * nam, BOP bop, double num){
  AstExp* ast = calloc(sizeof(AstExp), 1); 
  ast->kind = kind; 
  ast->next = next; 
  AstNam* lhs = calloc(sizeof(AstNam), 1); 
  lhs->kind = ASTNAM;
  lhs->next = NULL;   
  lhs->nam = nam; 
  ast->lhs = (Ast*) lhs; 
  ast->bop = bop; 
  AstNum* rhs = calloc(sizeof(AstNum), 1); 
  rhs->kind = ASTNUM; 
  rhs->next = NULL;
  rhs->num = num; 
  ast->rhs = (Ast*) rhs; 
  return ast; 
}

AstExp* constructSecondExample(AST kind, Ast* next, double num) {
  AstExp* ast = calloc(sizeof(AstExp), 1); 
  ast->kind = kind; 
  ast->next = next; 
  AstNum* lhs = calloc(sizeof(AstNum), 1); 
  lhs->kind = ASTNUM; 
  lhs->next = next; 
  lhs->num = num; 
  ast->lhs = (Ast*) lhs; 
  ast->bop = BOPNONE; 
  ast->rhs = NULL; 
}

AstExp* constructThirdExample(AST kind, Ast* next, char* nam, BOP bop, char* str){
  AstExp* ast = calloc(sizeof(AstExp), 1); 
  ast->kind = kind; 
  ast->next = next; 
  AstNam* lhs = calloc(sizeof(AstNam), 1); 
  lhs->kind = ASTNAM; 
  lhs->next = NULL; 
  lhs->nam = nam; 
  ast->lhs = (Ast*) lhs; 
  ast->bop = bop; 
  AstStr* rhs = calloc(sizeof(AstStr), 1); 
  rhs->kind = ASTSTR; 
  rhs->next = NULL;
  rhs->str = str;
  ast->rhs = (Ast*) rhs;
  return ast; 
}

AstExp* constructFourthExample(){
  AstExp* ast = calloc(sizeof(AstExp), 1); 
  ast->kind = ASTEXP; 
  ast->next = NULL; 
  ast->bop = BOPNONE; 
  ast->rhs = NULL; 

  AstStr* namlits = calloc(sizeof(AstStr), 1); 
  namlits->kind = ASTNAM; 
  namlits->next = NULL;   // Here, we could chain together the arguments. 
  namlits->str = "numItems"; 

  AstNam* nam = calloc(sizeof(AstNam), 1); 
  nam->kind = ASTNAM; 
  nam->next = NULL;
  nam->nam = "calc"; 

  AstCall* call = calloc(sizeof(AstCall), 1); 
  call->kind = ASTCALL; 
  call->next = NULL; 
  call->nam = nam; 
  call->namlits = (Ast*) namlits; 
  ast->lhs = (Ast*) call;
  return ast;
}
//-----------------------------------------------------------------------------//

int main(){
  /*
  Made a couple functions whose only purpose is to return node structures 
  that are equivalent to the ones in the example. 
  With the exception of the 'call' one (fourth), I made them so you can switch around some
  of the values. However, the structure of each one is static for the functions. 
  Eg. First one must be NAM BOP NUM 
  Eg. Second one must be NUM 
  Eg. Third one must be NAM BOP STRING 
  Eg. Fourth one must have a valid Call node. 

  !!Note!!: visitExp works with other valid AstExp * nodes. These functions just helped with testing. 
  I assume there'll be a set list of generated of AstExp nodes that'll be used for testing. 
  */
  printf("Running tests on all four examples for visitExp(...)\n");
  AstExp* firstExample = constructFirstExample(ASTEXP, NULL, "count", BOPMUL, 23);
  AstExp* secondExample = constructSecondExample(ASTEXP, NULL, 42);  
  AstExp* thirdExample = constructThirdExample(ASTEXP, NULL, "hello", BOPEEQ, "hello");
  AstExp* fourthExample = constructFourthExample(); 
  visitExp(firstExample); 
  visitExp(secondExample); 
  visitExp(thirdExample); 
  visitExp(fourthExample); 
}