#include <stdio.h>
#include <stdlib.h> 
#include <string.h>

/*
Extension
Task 1: Allow spaces - E.g "2 + 3 "
Task 2: Allow multi-digit numbers. E.g Numbers greater than 9 or negatives following the same rule.
Task 3: Allow all operators. E.g +, -, *, /
Task 4: Allow real number. E.g. 1.2 * 25.7
Task 5. Support multi operator operations. E.g 1 + 2.3 * 4.5 - 6.0. No precedence ok.
*/

/*
Hints:
Use scanf, gets, strtok 
Beware: "gets" is inherently unsafe because it causes buffer overruns and security failures. 
*/

//Some large value to be able to handle most inputs. 
#define BUFF_SIZE 256 
#define VALID 0
#define INVALID 1

#define FIRST 0 
#define SECOND 1
/*
void processInput(char * input){
    char c; 
    printf("%s", input);
    char operator; 

    int num1 = 0; 
    int num2 = 0;
    int ans = 0; 
    
    //Left-side of operator number or right-side. 
    //E.g. if an operator is found, we should start adding to the right side number. 
    int whichNum = FIRST;
    
    int operatorIsValid = VALID; //To stop multiple operators in a row. 
    // E.g. 1 ** 3, 1 +/ 5, etc. 

    int completeNum = VALID;  //a complete number counts as one without spaces inbetween. 
    //E.g 14 + 3 is valid. But 1 4 + 3 is not. This is used to keep track of this.  
    //If completeNum is invalid, the next non-whitespace character should be a operator. 
    
    for (int i = 0; i < strlen(input); i++){
        c = input[i]; 
        //Skip over spaces. 
        if (c == ' ' || c == '\t') {
            completeNum = INVALID;
            continue; 
        }  else if (c > '0' && c < '9') { //Checking for valid input. 
            if (completeNum == VALID){
                if (whichNum == FIRST){
                    num1 *= 10; 
                    num1 += c - '0'; 
                } else if (whichNum == SECOND){
                    num2 *= 10; 
                    num2 += c - '0'; 
                }
            } else {
                printf("Invalid input. Spaces in-between numbers. \n");
                return; 
            }
            printf("%c", c);
        } else if (c == '-' || c == '+' || c == '*' || c == '/'){
            if (operatorIsValid == VALID) {
                whichNum = SECOND; 
                operatorIsValid = INVALID; 
                if (c == '-') {
                    operator = '-'; 
                } else if (c == '+') {
                    operator = '+'; 
                } else if (c == '*'){
                    operator = '*'; 
                } else {
                    operator = '/';
                }
            } else if (operatorIsValid == INVALID && whichNum == SECOND){
                //Do calculations here 
                operatorIsValid = VALID; 
                whichNum = FIRST;   
            } else {
                printf("Multiple operators in a row. Please reformat.\n");
                return; 
            }

        } else if (c == '\0' || c == '\n'){ //Do calculations here. 
            if (whichNum == SECOND && operatorIsValid == INVALID){
                //Do calculation

                whichNum = FIRST;
                operatorIsValid = VALID; 
            }
            return; 
        } else {
            printf("%c", c);
            printf("Invalid input entered. Please re-enter only numbers and operands. \n");
            return;  
        }
    }
}*/
int processInput(char * input){

    return 0; 
}

//Got this method from https://stackoverflow.com/questions/1726302/removing-spaces-from-a-string-in-c
//Removes white space from a string. 
void remove_whitespace(char * s){
    const char* d = s;
    do {
        while (*d == ' ' || *d == '\t') {
            ++d;
        }
    } while (*s++ = *d++);
}

void processHelper(char * input){
    remove_whitespace(input); 
    int sum = 0; 
    char num1[100]; 
    char num2[100]; 
    char operator; 
    char c; 
    printf("%s", input);
    while ((strstr(input, "+") != NULL) || (strstr(input, "*") != NULL) 
    || (strstr(input, "-") != NULL) || (strstr(input, "/") != NULL)){
        for (int i = 0; i < strlen(input); i++){
            c = input[i]; 

        }
        return; 
    }
}

//ASCII 0 - 9 is 48 - 57
void runShell() {
    int num1 = 0; 
    int num2 = 0; 
    char input[BUFF_SIZE]; 

  //Infinite loop until 'quit' is entered. 
    while (1){
        printf("? ");
        if (fgets(input, BUFF_SIZE, stdin) != NULL){
            processHelper(input); 
            printf("\n");
        } else {
            printf("Input is too long. Please reformat and try again!\n");
            continue; 
        }
        fflush(stdin); 
  }
}

int main() {
    printf("Enter 'quit' (case sensitive) to exit)\n");
    runShell(); 
}