#ifndef PARSE_H_   /* Include guard */
#define PARSE_H_

#include "constants.h"

//int foo(int x);  /* An example function declaration */

void split(char *cmd, char **args);
char* skipWhiteSpace(char *srcPtr); 
void check_parse();
void printArgs(char *args[], size_t n);

#endif 
