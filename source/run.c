#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
 
#include <sys/types.h>
#include <sys/wait.h>
 
#include "constants.h"

/* The array below will hold the arguments: args[0] is the command. */
char* args[512];
pid_t pid;
/* 
 *  < unit tests
 */

void print_run_check(char* fileName, int redir_num){
   
      printf("filename: %s\n",fileName);
      printf("redir number: %d\n",redir_num);
}

void check_run()
{
   char* fileName = (char*)malloc(sizeof(char)*50);
   char cmd[] = "     test >> ls.txt\n";
   int redir_num = 0;
   redir_num = getRedir(cmd, fileName);
   print_run_check(fileName, redir_num);
}
/* 
 *   / >
 */
 

int pipeCommand(int input, int first, int last){
	int pipettes[2];
 
	/* Invoke pipe */
	pipe( pipettes );	
	pid = fork();
 
	/*
	 SCHEME:
	 	STDIN --> O --> O --> O --> STDOUT
	*/
 
	if (pid == 0) {
		if (first == 1 && last == 0 && input == 0) {
			// First command
			dup2( pipettes[PIPE_WRITE], STDOUT_FILENO );
		} else if (first == 0 && last == 0 && input != 0) {
			// Middle command
			dup2(input, STDIN_FILENO);
			dup2(pipettes[PIPE_WRITE], STDOUT_FILENO);
		} else {
			// Last command
			dup2( input, STDIN_FILENO );
		}
 
		if (execvp( args[0], args) == -1)
			_exit(EXIT_FAILURE); // If child fails
	}
 
	if (input != 0) 
		close(input);
 
	// Nothing more needs to be written
	close(pipettes[PIPE_WRITE]);
 
	// If it's the last command, nothing more needs to be read
	if (last == 1)
		close(pipettes[PIPE_READ]);
 
	return pipettes[PIPE_READ];
}
 
/*
int bgCommand(int input, int first, int last){
	
	pid = fork();
 
	if (pid == 0) {
		if (first == 1 && last == 0 && input == 0) {
			// First command
			//dup2( pipettes[PIPE_WRITE], STDOUT_FILENO );
		} else if (first == 0 && last == 0 && input != 0) {
			// Middle command
			dup2(input, STDIN_FILENO);
			dup2(pipettes[PIPE_WRITE], STDOUT_FILENO);
		} else {
			// Last command
			dup2( input, STDIN_FILENO );
		}
 
		if (execvp( args[0], args) == -1)
			_exit(EXIT_FAILURE); // If child fails
	}
 
	if (input != 0) 
		close(input);
 
	// Nothing more needs to be written
	close(pipettes[PIPE_WRITE]);
 
	// If it's the last command, nothing more needs to be read
	if (last == 1)
		close(pipettes[PIPE_READ]);
	
	return STANDARD_INPUT;
}
*/

int run(char* cmd, int input, int first, int last, int* n, int delimitFlag){
	split(cmd, args);
	if (args[0] != NULL) { /*
		if (strcmp(args[0], EXIT) == 0) 
			exit(0); */
		(*n) += 1;
		if(delimitFlag == AMPERSAND_NUM){ 
		  //return bgCommand(input, first, last);
		  
		}
		return pipeCommand(input, first, last);
	}
	return 0;
}

/*
 * OUTPUT_REDIRECTION_NUM 
 * APPEND_OUTPUT_REDIRECTION_NUM 
 * INPUT_REDIRECTION_NUM 
 * NO_IO_REDIRECTION_NUM
 */

int getRedir(char* cmd_org, char* fileName){
  char redirStr[] = "><";
  char* cmd = (char*)malloc(sizeof(char)*50);
  char* backupPointer = cmd;
  strcpy ( cmd, cmd_org );
  char* next = strpbrk(cmd,redirStr);
  int redir_num = 0;
  
  if(*next == '<'){
    redir_num = INPUT_REDIRECTION_NUM;
    *next = '\0';
    *(next + 1)= '\0';
    cmd = next + 2;
  } else if(*next == '>'){
    if((*next == '>') && (*(next + 1) == '>')){
      redir_num = APPEND_OUTPUT_REDIRECTION_NUM;
      *next = '\0';
      *(next + 1)= '\0';
      *(next + 2)= '\0';
      cmd = next + 3;
    } else {
      redir_num = OUTPUT_REDIRECTION_NUM;
      *next = '\0';
      *(next + 1)= '\0';
      cmd = next + 2;
    }
  } else {
    redir_num = NO_IO_REDIRECTION_NUM;
  }
  
  if(redir_num != NO_IO_REDIRECTION_NUM){
    next = strpbrk(cmd," \n");
    *next = '\0';
    strcpy(fileName, cmd);
  }
  free(backupPointer);
  return redir_num;
}


 

 
