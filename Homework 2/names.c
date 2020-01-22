/*
Ji Kang 
Homework #2: Part 4
Write a short C program to extract the names (or identifiers) 
from the following string:
char text[] = “It was    a     dark and     stormy   night  ”;
*/
#include <regex.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

//There's 7 names to be found in the string 
const size_t maxMatches = 7;
//Regex pattern only has 1 matchable group (the full match) 
const size_t maxGroups = 1; 

void findIdentifiers(regex_t regex, char *text){
    regmatch_t matches[maxMatches];
    //Iterates over the given string 
    char * cursor = text;  
    for (int i = 0; i < maxMatches; i++){
        //Finds the first token that matches the given pattern. Breaks if none exists.  
        if (regexec(&regex, cursor, maxGroups, matches, 0)){
            break; 
        }
        //What to increment the cursor by 
        int offset = 0; 
        //If the offset of the substring in the beginning is -1, break. 
        //No remaining multiple matches. 
        if (matches[0].rm_so == (size_t) -1){
            break; 
        }
        /*
        https://www.gnu.org/software/libc/manual/html_node/Regexp-Subexpressions.html
        Offset needs to be the "offset" in the string of the beginning of the substring. 
        Adding this value to the string gets the address of the first match. 
        Skips over whitespace this way. 
        */
        offset = matches[0].rm_eo; 
        char copy[strlen(cursor) + 1];
        strcpy(copy, cursor); 
        copy[matches[0].rm_eo] = 0; 
        printf("Identifier Found: %s\n", copy + matches[0].rm_so);
        cursor += offset; 
    }
}

int main(){
    regex_t regex;
    char *text = "   It was    a     dark and     stormy   night  ";
    char pattern[] = "([a-zA-Z_][a-zA-Z0-9_]*)";
    int statusCode = regcomp(&regex, pattern, REG_EXTENDED); 
    if (statusCode){
        printf("Regex compilation failed.\n");
        exit(EXIT_FAILURE);
    }
    findIdentifiers(regex, text);
}

