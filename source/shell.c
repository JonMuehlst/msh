#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "constants.h"
#include "parse.h"
#include "errorcheck.h"

/*	Functions
 * 
 * 
 */

/* Final cleanup, 'wait' for processes to terminate.
 *  n : Number of times 'command' was invoked.
 */
static void cleanup(int n)
{
	int i;
	for (i = 0; i < n; ++i) 
		wait(NULL); 
}
 
int main(int argc, char *argv[]){
  
  char line[1024] = "exit\n";
  char lineBCKP[1024];
  int n = 0; /* number of calls to 'command' */
  int i = 0;
  while(i++<3) {
    //check_all_errorcheck();
    printf("\n");
    //check_parse();
    printf("\n");
    printf("$ ");
    
    if(!fgets(line, 1024, stdin)){
      printf("\n");
      break;
    } 
    /* Remove the trailing newline 
    line[strlen(line) - 1] = '\0'; */
    
    if(strcmp(EXITN,line) == 0){
      break;
    }
    memcpy ( lineBCKP, line, strlen(line)+1 );
    
    int input = 0;
    int first = 1;
    
    int status = 0;
    int delimitFlag = 0;
    pid_t pid = 0;
    char* cmd = line;
    char* next = strpbrk(cmd, DELIMITERS); /* Find first '|' or '&' */
    
    pid = fork();
    
    if(pid == 0){  
	while (next != NULL) {
		/* 'next' points to '|' or '&' */
		if(*next == '|') delimitFlag = PIPE_NUM;
		if(*next == '&') delimitFlag = AMPERSAND_NUM;
		//if(*next == '\n') delimitFlag = NEWLINE_NUM;
		*next = '\0';
		input = run(cmd, input, first, 0, &n);
		
		cmd = next + 1;
		next = strpbrk(cmd, DELIMITERS); /* Find next '|' or '&' */
		first = 0;
	}
	
	input = run(cmd, input, first, 1, &n);
	cleanup(n);
	n = 0;
    } else if(lineBCKP[strlen(lineBCKP) - 2] == '&') {
	waitpid(pid, &status, WNOHANG);
    } else {
	wait(pid, &status, 0);
    }
      
  }
  
} 


    