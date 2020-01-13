#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <stdlib.h>
#include <stdbool.h> 
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

//Char buffers used to represent the operands. 
char num1[100]; 
char num2[100]; 

//Hold the current operator. 
char operator; 

//Methods for doing arithmetic. 
double add(double num1, double num2) { return num1 + num2; }
double subtract(double num1, double num2){ return num1 - num2; }
double multiply(double num1, double num2){ return num1 * num2; }
double divide(double num1, double num2) { return num1 / num2; }

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

/*
Purpose is to determine if there is a expression to evaluate. 
This just checks to see if the input has a operator (+, -, *, and/or /) inside
and will return true if so, false otherwise. 
*/
bool isExpression(char * input){
    if ((strstr(input, "+") != NULL) || (strstr(input, "*") != NULL) 
        || (strstr(input, "-") != NULL) || (strstr(input, "/") != NULL)) {
            return true; 
    }
    return false; 
}

//Just reset the char buffers 
void clearNums(){
    memset(num1, 0, sizeof(num1)); 
    memset(num2, 0, sizeof(num2)); 
}

/*
Test Cases: 
1. Allow spaces Eg: “ 2  + 3   “ --> Answer: 5
2. Allow Multi-digit numbers    Eg: “ 42 - 39  “ --> Answer: 3
3. Support:  + - * /    Eg: “ 3 * 67  “ --> Answer: 201
4. Allow real numbers   Eg: “ 1.2 * 25.7” --> Answer: 30.84
5. Support multi-operations Eg: “  1 + 2.3* 4.5 – 6.0 “ (no precedence!)
     --> Answer: 8.85 (without precedence)
*/

/*
Main method for handling the arithmetic expressions. 
Does so by taking the substrings of the first operand, operator, and the second operand. 
It will evaluate this expression then subsitute the answer back into the main string. 
E.g. 1 + 3 * 3 would evaluate (1 + 3) as 4 then subsitute it back into the input string
forming 4 * 3, then recalling the method to evaluate this new string. 
It'll do this evaluation, subsitution, and method call until there is no expression
to be evaluated - in which it assumes that the correct answer has been found. 
A lot of error checking has not been implemented. E.g. Spaces in-between numbers, 
multiple expressions in a row ( 1 +++*--/ 4), etc. 
*/
void processHelper(char * input){
    //Remove whitespace. 
    remove_whitespace(input);
    char c;     // char variable to hold which char value we're at in the string. 
    int prevInd = 0; // Index representing the lower bound of the string. 
    int whichNum = FIRST;   // Dictates if we're finding the first or second operand. 

    bool firstOperator = true; // Used to take the substring of 1st operand, op, 2nd operand. 
    if (isExpression(input)) {
        for (unsigned int i = 0; i < strlen(input); i++) {
            c = input[i]; 
            //The length of the substring to be extracted. 
            int size = i - prevInd; 
            //Check if the number is within the numerical range in ascii values. 
            if (c < '0' || c > '9') {
                // If it's not, it'll check if it's an operator or a decimal. 
                switch(c){
                    case '.':
                        continue; 
                    case '+':
                        if (firstOperator){
                            operator = '+';
                            firstOperator = false; 
                        }
                        break; 
                    case '-':
                        if (firstOperator){
                            firstOperator = false; 
                            operator = '-';
                        }
                        break;
                    case '*':
                        if (firstOperator){
                            firstOperator = false; 
                            operator = '*';
                        }
                        break;
                    case '/':
                        if (firstOperator){
                            firstOperator = false; 
                            operator = '/';
                        }
                        break;
                    case '\n':
                    case '\0':
                        break;
                    default:
                        printf("Invalid character in input: '%c'. Please reformat and re-enter.\n", c); 
                        return; 
                }     
                // First operand is extracted from the previous index to prevInd + size. 
                if (whichNum == FIRST){
                    //Copy into our num1 variable. 
                    memcpy(num1, &input[prevInd], size); 
                    //Null byte to note the end of the string. 
                    num1[size] = '\0';
                    //Increment the prevInd variable. 
                    prevInd = i + 1;
                    //Change to check for the second operand. 
                    whichNum = SECOND; 
                    continue; 
                } else {
                    //Copy into our num2 variable. 
                    memcpy(num2, &input[prevInd], size); 
                    //Null byte ot note the end of  the string
                    num2[size] = '\0';
                    //Increment variable ()
                    prevInd = i + 1;

                    double firstNum = atof(num1); 
                    double secNum = atof(num2); 

                    double ans = 0.0; 
                    if (operator == '+') {
                        ans = add(firstNum, secNum);
                    } else if (operator == '-') {
                        ans = subtract(firstNum, secNum); 
                    } else if (operator == '*'){
                        ans = multiply(firstNum, secNum);  
                    } else {
                        ans = divide(firstNum, secNum);
                    }

                    char temp[BUFF_SIZE]; 
                    char substring[BUFF_SIZE]; 
                    snprintf(temp, sizeof(ans), "%.5f", ans);
                    memcpy(substring, &input[prevInd - 1], strlen(input)); 

                    char * n = strcat(temp, substring);
                    //n[strlen(n) - 2] = '\0';
                    //printf("first num: %s, second num: %s\n", num1, num2);
                    //printf("------------\n%f\n%s\n%s\n------------\n", ans, substring, n); 
                    clearNums();
                    if (isExpression(n)){
                        processHelper(n);
                        return;
                    } else {
                        printf("The answer is %.3f\n", ans);
                        return;
                    }

                }
            } 
        }
    } 
}

//ASCII 0 - 9 is 48 - 57
void runShell() {
    char input[BUFF_SIZE]; 
  //Infinite loop until 'quit' is entered. 
    while (1){
        printf("? ");
        if (fgets(input, BUFF_SIZE, stdin) != NULL){
            if (strstr(input, "quit") != NULL && strlen(input) == 5){
                printf("Goodbye!\n");
                exit(EXIT_SUCCESS);
            } 
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
