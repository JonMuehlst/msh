#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <errno.h>

#include "constants.h"
#include "parse.h"
#include "errorcheck.h"
#include "handler.h"
#include "global.h"

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
  
  char line[1024] = "kate & ls | grep l"; // = "ls \n";
  
  int n = 0; /* number of calls to 'command' */
  int i = 0;
  pid_t pid = 0;
  
  /* install SIGCHLD handler */
  signal(SIGCHLD, sigchld_handler);
  //signal(SIGCHLD, SIG_IGN); // prehaps use this?
  signal(SIGINT, SIG_IGN); // ignore CTRL - C
  
  printf("Press t to test or any other key to continue...\n");
  printf("$ ");
    
    if(!fgets(line, 1024, stdin)){
      printf("\n");
      exit(0);
    }
  
  while(1) {
    
    if(line[0] == 't'&& line[1] == '\n'){
      //strcpy(line ,"kate & ls | grep l\n"); //check mixed commands
      //check_all_errorcheck();
      //printf("\n");
      //check_parse();
      //check_run();
      //printf("\n");
      printf("No tests chosen in source file\n");
      break;
    
      
    } else {
      
	printf("$ ");
	
	if(!fgets(line, 1024, stdin)){
	  printf("\n");
	  break;
	} 
	int c = chck_general_error(line);
	if(c == -1){
	  perror("invalid input");
	  writeToLogFile(line, "invalid input", EIO);
	  printf("\n");
	  continue;
      }
    }
    
    if(strcmp(EXITN,line) == 0){
      break;
    }
    strcpy ( lineBCKP, line);
    
    int input = 0;
    int first = 1;
    
    int trailingAmp = 0;
    int status = 0;
    int delimitFlag = 0;
    
    char* cmd = line;
    char* next = strpbrk(cmd, DELIMITERS); /* Find first '|' or '&' */
    
    
    while (next != NULL) {
	    /* 'next' points to '|' or '&' */
	    if(*next == '|') 
	      delimitFlag = PIPE_NUM;
	    if(*next == '&'){ 
	      delimitFlag = AMPERSAND_NUM;
	      if(*(next + 1) == '\n'){ 
		trailingAmp = 1;
		//printf("trailing amp\n");
	      }
	    }
	    *next = '\0';
	    input = run(cmd, input, first, 0, &n, delimitFlag);
	    cmd = next + 1;
	    next = strpbrk(cmd, DELIMITERS); /* Find next '|' or '&' */
	    first = 0;
    }
    if(!trailingAmp)
      input = run(cmd, input, first, 1, &n, NO_BLOCK_ENDING);
    //cleanup(n);
    n = 0;
     
  }
  
} 


    