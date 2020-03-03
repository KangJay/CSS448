// ut.c - Utility functions for the Tog compiler - Jim Hogg, 2019

#include "ut.h"

void utDie2Str(char* func, char* msg) {
  printf("\n\nERROR: %s: %s \n\n", func, msg);
  utPause();
}


void utDie3Str(char* func, char* msg1, char*msg2) {
  printf("\n\nERROR: %s: %s %s \n\n", func, msg1, msg2);
  utPause();
}


void utDie2StrCharLC(char* func, char* msg, char c, int linNum, int colNum) {
  printf("\n\nERROR: %s %s %c at (%d, %d) \n\n",
    func, msg, c, linNum, colNum);
  utPause();
}


void utDieStrTokStr(char* func, Tok* tok, char* msg) {
  printf("\n\nERROR: %s: Found %s but expecting %s at (%d, %d) \n\n",
    func, tokStr(tok->kind), msg, tok->linNum, tok->colNum);
  utPause();
}


void utDie4Str(char* func, char* msg1, char* msg2, char* msg3) {
  printf("\n\nERROR: %s: %s %s %s \n\n", func, msg1, msg2, msg3);
  utPause();
}


void utPause() {
  printf("Hit any key to finish");
  getchar();
  exit(0);
}

char* utReadFile(char* filePath) {
  FILE *fileInput = fopen(filePath, "r"); 

  //++ Use fopen.  If operation failes, tell user and stop.
  if (!fileInput){
    //If the file doesn't exist or cannot be opened/read from. 
    return NULL; 
  }
  int fileSize; 
  char *buffer; 

  //++ Find how big the file is.  Call it fileSize.  Use fseek, ftell, fseek.
  fseek(fileInput, 0, SEEK_END); 
  fileSize = ftell(fileInput); 
  fseek(fileInput, 0, SEEK_SET); 
  //printf("File: \'%s\' has a size of %d bytes.\n", filePath, fileSize); 
  
  //++ Allocate a buffer, zero-filled, to hold the file contents. 
  buffer = (char *)calloc(fileSize, sizeof(char)); 
  if (!buffer){
    printf("Memory error has occurred. Please retry.\n");
    exit(EXIT_FAILURE); 
  }
  //++ Read entire file into the 'prog' buffer
  fread(buffer, sizeof(char), fileSize, fileInput);   
  fclose(fileInput); // Close file. 
  free(fileInput);  // Free pointer's allocated memory.
  return buffer;
}


char* utStrndup(char* s, int len) {
  char* copy = malloc(len + 1);
  strncpy(copy, s, len);
  copy[len] = '\0';
  return copy;
}
