#include <stdio.h> 
#include <stdlib.h>
#include <stdbool.h>
#include <string.h> 

/*
HOKUM GRAMMAR RULES: 
1. Prog  => Stm ; Prog | Stm      // Program
2. Stm   => Asg | If              // Statement
3. Asg   => Var = Exp             // Assignment
4. If    => if Exp then Asg       // If
5. VorC  => Var | Const           // Variable or Constant
6. Exp   => VorC | VorC + VorC    // Expression
7. Var   => [a-z]                 // Variable
8. Const => [0-9]                 // Constant
*/


typedef enum { 
    TOKVAR = 1, TOKEQ, TOKCONST, 
    TOKSEMI, TOKADD, TOKIF, TOKTHEN 
} TokKind;


char prog[] = "a = 3; b = a + a; if a then x = a + 5;";      // not used

/*
TokKind toks[] =
  { TOKVAR, TOKEQ,  TOKCONST,  TOKSEMI,                     // a = 3;
    TOKVAR, TOKEQ,  TOKVAR,    TOKADD,  TOKVAR,   TOKSEMI,  // b = a + a;
    TOKIF,  TOKVAR, TOKTHEN,                                // if a then
    TOKVAR, TOKEQ,  TOKVAR,    TOKADD,  TOKCONST, TOKSEMI   // x = a + 5;
  };
*/

TokKind toks[] =
  { TOKVAR, TOKEQ,  TOKCONST,  TOKEQ,                     // a = 3;
    TOKVAR, TOKEQ,  TOKVAR,    TOKADD,  TOKVAR,   TOKSEMI,  // b = a + a;
    TOKIF,  TOKVAR, TOKTHEN,                                // if a then
    TOKVAR, TOKEQ,  TOKVAR,    TOKADD,  TOKCONST, TOKSEMI   // x = a + 5;
  };
  
int tokNum = 0;     // current Token number within toks
int hiTokNum = 18;  // highest Token number within toks


void mustbe(TokKind k);
void pause();   //  Done 
void rexAsg();  
void rexExp();
void rexIf();
void rexProg();
void rexStm();
void rexVorC();
TokKind getTokKind() { return toks[tokNum]; } 
bool isVar(TokKind t){ return t == TOKVAR; }
bool isConst(TokKind t){ return t == TOKCONST; }

char * getTokString(TokKind t) {
    char * retString;
    switch(t){
        case TOKVAR: 
            retString = "TOKVAR";
            break; 
        case TOKEQ:
            retString = "TOKEQ"; 
            break; 
        case TOKCONST:
            retString = "TOKCONST"; 
            break; 
        case TOKSEMI:
            retString = "TOKSEMI"; 
            break; 
        case TOKADD:
            retString = "TOKADD"; 
            break; 
        case TOKIF:
            retString = "TOKIF"; 
            break; 
        case TOKTHEN: 
            retString = "TOKTHEN";
            break; 
    } 
    return retString;
}

int main(){
    rexProg();
    printf("Success! Hokum program is valid \n"); 
    pause(); 
}

void rexIf(){
    //If    => if Exp then Asg
    mustbe(TOKIF); // Checking for 'if' 
    rexExp(); 
    mustbe(TOKTHEN); // Checking for 'then'
    rexAsg(); 
}

void rexVorC(){
    //  Var | Const
    TokKind t = getTokKind();  
    if (isVar(t)){
        mustbe(TOKVAR); 
    } else if (isConst(t)){
        mustbe(TOKCONST);
    } else {
        printf("Error: want %s or %s, but found %s \n", getTokString(TOKVAR), getTokString(TOKCONST), getTokString(t)); 
        pause(); 
    }
}

void rexExp(){
    //VorC | VorC + VorC
    rexVorC(); 
    //Check one token ahead to see if it's the left rule or right rule
    if (getTokKind() != TOKADD) return; 
    mustbe(TOKADD); // Checking for '+' 
    rexVorC(); 
}

void rexAsg(){
    //Asg => Var = Exp
    mustbe(TOKVAR); 
    mustbe(TOKEQ);
    rexExp();
}

void rexStm(){
    //Stm   => Asg | If 
    if (getTokKind() == TOKIF) {
        rexIf(); 
    } else {
        rexAsg(); 
    }
    //If the token is invalid, it'll be caught in 'rexIf' or 'rexAsg'
}

void rexProg(){
    //Stm ; Prog | Stm
    rexStm(); 
    while(tokNum <= hiTokNum) {
        mustbe(TOKSEMI); // Checking for ';'
        if (tokNum > hiTokNum) break;
        rexProg(); 
    }
}

void mustbe(TokKind k){
    TokKind t = toks[tokNum]; 
    if (t == k){
        ++tokNum;
    } else {
        printf("Error: want %s, but found %s \n", getTokString(k), getTokString(t)); 
        pause(); 
    }
}

void pause(){ 
    printf("Press the 'Enter' key to exit."); 
    getchar();
    printf("Goodbye!\n"); 
    exit(EXIT_SUCCESS);
}