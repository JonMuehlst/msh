/*
 * parse.c
 */

#include <stdio.h>
#include <string.h>
#include "parse.h"

/* 
 *  < unit tests
 */

void printArgs(char *args[], size_t n){
  
   size_t i = 0;
   for (; i < n; ++i)
   {
      printf("%s\n",args[i]);
   } 
}

void check_parse()
{
   static char* args[512];
   char cmd[] = "     ls | test >> ls.txt & ls\n";
   
   split(cmd, args);
   printArgs(args, 7);
}
/* 
 *   / >
 */

char* skipWhiteSpace(char *srcPtr)
{
	while((*srcPtr == ' ' || *srcPtr == '\t' || *srcPtr == '\n') && *srcPtr)
	{
		srcPtr++;
	}
	return srcPtr;
}

void split(char *cmd, char **args)
{
	cmd = skipWhiteSpace(cmd);
	char* next = strchr(cmd, ' ');
	int i = 0;
 
	while(next != NULL) {
		next[0] = '\0';
		args[i] = cmd;
		++i;
		cmd = skipWhiteSpace(next + 1);
		next = strchr((cmd), ' ');
	}
 
	if (cmd[0] != '\0') {
		args[i] = cmd;
		next = strchr(cmd, '\n');
		next[0] = '\0';
		++i; 
	}
 
	args[i] = NULL;
} 

