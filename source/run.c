#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
 
#include "constants.h"
#include "run.h"
#include "global.h"

/* The array below will hold the arguments: args[0] is the command. */
char* args[512];
char fileName[512];
int redir_num;
int status;
FILE* fp;
int fd;
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
   //fileName = (char*)malloc(sizeof(char)*50);
   char* bckpPointer = fileName;
   char cmd[] = "     test >> ls.txt\n";
   int redir_num = 0;
   getRedir(cmd);
   print_run_check(fileName, redir_num);
   //free(fileName);
}
/* 
 *   / >
 */
 

int pipeCommand(int input, int first, int last){
	int pipettes[2];
	
	/* Invoke pipe */
	pipe( pipettes );	
	pid = fork();
	if (pid == -1) {
	  int errnum = errno;
	  perror("fork");
	  writeToLogFile(lineBCKP, "fork", errnum);
	  exit(EXIT_FAILURE);
	}
	
	/*
	 SCHEME:
	 	STDIN --> O --> O --> O --> STDOUT
	*/
 
	if (pid == 0) {
		if (first == 1 && last == 0 && input == 0) {
			// First command
			dup2( pipettes[PIPE_WRITE], STDOUT_FILENO );
		} else if (first == 1 && last == 0 && input != 0) {
			// First command
			dup2(input, STDIN_FILENO);
			dup2( pipettes[PIPE_WRITE], STDOUT_FILENO );
		} else if (first == 0 && last == 0 && input != 0) {
			// Middle command with input redirection
			dup2(input, STDIN_FILENO);
			dup2(pipettes[PIPE_WRITE], STDOUT_FILENO);
		} else if (first == 0 && last == 0 && input == 0) {
			// Middle command without input redirection
			//dup2(input, STDIN_FILENO);
			dup2(pipettes[PIPE_WRITE], STDOUT_FILENO);
		} 
		
		else {
			// Last command
			dup2( input, STDIN_FILENO );
		}
		
		closeFile();
 
		if (execvp( args[0], args) == -1){
			int errnum = errno;
			perror("execvp failed");
			writeToLogFile(lineBCKP, "execvp failed", errnum);
			exit(EXIT_FAILURE); // If child fails
		}
	} else {
	  int w = waitpid(pid, &status, WNOHANG );
	  if(w == -1){
	    int errnum = errno;
	    perror("Child process terminated with errors");
	    writeToLogFile(lineBCKP, "Child process terminated with errors", errnum);
	    exit(EXIT_FAILURE);
	  }
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
 

int bgCommand(int input, int first, int output){
	
	int status = 0;
	pid = fork();
	if (pid == -1) {
	  int errnum = errno;
	  perror("fork");
	  writeToLogFile(lineBCKP, "fork", errnum);
	  exit(EXIT_FAILURE);
	}
 
	if (pid == 0) {
		if (first == 1 && input == 0 && output == STANDARD_OUTPUT) {
			// First command with standard IO
		} else if (input != 0 && output == STANDARD_OUTPUT) {
			dup2(input, STDIN_FILENO);
		} else if (input == 0 && output != STANDARD_OUTPUT) {
			dup2(output, STDOUT_FILENO);
		} //if first == 0 && input == 0 && output == 0 then don't redirect...
 
		closeFile();
  
		if (execvp( args[0], args) == -1){
			int errnum = errno;
			perror("execvp failed");
			writeToLogFile(lineBCKP, "execvp failed", errnum);
			exit(EXIT_FAILURE); // If child fails
		}
	} else {
	  int w = waitpid(pid, &status, WNOHANG );
	  if(w == -1){
	    int errnum = errno;
	    perror("Child process terminated with errors");
	    writeToLogFile(lineBCKP, "Child process terminated with errors", errnum);
	    exit(EXIT_FAILURE);
	  }
	}
	
	if (input != 0) 
		close(input);
 
	return STANDARD_INPUT;
}

int command(int input, int first, int output){
	
	int status = 0;
	pid = fork();
	if (pid == -1) {
	  int errnum = errno;
	  perror("fork");
	  writeToLogFile(lineBCKP, "fork", errnum);
	  exit(EXIT_FAILURE);
	}
 
	if (pid == 0) {
		if (first == 1 && input == 0 && output == STANDARD_OUTPUT) {
			// only command
		} else if (input != 0 && output == STANDARD_OUTPUT) {
			// last command
			dup2(input, STDIN_FILENO);
		} else if (input == 0 && output != STANDARD_OUTPUT){
			dup2(output, STDOUT_FILENO);
		} else if (input != 0 && output != STANDARD_OUTPUT){
			dup2(input, STDIN_FILENO);
			dup2(output, STDOUT_FILENO);
		}
		
		closeFile();
		
		if (execvp( args[0], args) == -1){
			int errnum = errno;
			perror("execvp failed");
			writeToLogFile(lineBCKP, "execvp failed", errnum);
			exit(EXIT_FAILURE); // If child fails
		}
	} else {
	  int w = waitpid(pid, &status, 0 );
	  if(w == -1){
	    int errnum = errno;
	    perror("Child process terminated with errors");
	    writeToLogFile(lineBCKP, "Child process terminated with errors", errnum);
	    exit(EXIT_FAILURE);
	  }
	}
	
	if (input != 0) 
		close(input);
 
	return STANDARD_INPUT;
}

/*
 * OUTPUT_REDIRECTION_NUM 
 * APPEND_OUTPUT_REDIRECTION_NUM 
 * INPUT_REDIRECTION_NUM 
 * NO_IO_REDIRECTION_NUM
 */

int run(char* cmd, int input, int first, int last, int* n, int delimitFlag){
	
	int output = STANDARD_OUTPUT;
	
	getRedir(cmd); //redir_num and fileName get assigned here.
	
	if(redir_num == OUTPUT_REDIRECTION_NUM || redir_num == APPEND_OUTPUT_REDIRECTION_NUM){
	  openFile();
	  output = fd;
	} else if(redir_num == INPUT_REDIRECTION_NUM){
	  openFile();
	  input = fd;
	}
	
	split(cmd, args);
	
	if (args[0] != NULL) { /*
		if (strcmp(args[0], EXIT) == 0) 
			exit(0); */
		(*n) += 1;
		if(delimitFlag == AMPERSAND_NUM){
		  return bgCommand(input, first, output);
		} else if(delimitFlag == PIPE_NUM){
		    return pipeCommand(input, first, last);
		} else if(delimitFlag == NO_BLOCK_ENDING){
		    return command(input, first, output);
		}
	}
	return 0;
}

void openFile(){
  if(redir_num == OUTPUT_REDIRECTION_NUM){
    fp = fopen(fileName, "w");
    fd = fileno(fp);  
  } else if(redir_num == APPEND_OUTPUT_REDIRECTION_NUM){
    fp = fopen(fileName, "a");
    fd = fileno(fp);
  } else if(redir_num == INPUT_REDIRECTION_NUM){
    fp = fopen(fileName, "r");
    if(fp == NULL){
      int errnum = errno;
      perror("Error opening file");
      writeToLogFile(lineBCKP, fileName, errnum);
      exit(EXIT_FAILURE);
    }
    fd = fileno(fp);
  } 
}

void closeFile(){
  if(redir_num != NO_IO_REDIRECTION_NUM)
    fclose(fp);
}

void getRedir(char* cmd){
  char redirStr[] = "><";
  char* next = strpbrk(cmd,redirStr);
  char* bP = cmd;
  redir_num = 0;
  
  if( next == NULL ) {
    redir_num = NO_IO_REDIRECTION_NUM;
  } else if(*next == '<'){
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
  } 
  
  if(redir_num != NO_IO_REDIRECTION_NUM){
    next = strpbrk(cmd," \n");
    *next = '\0';
    strcpy(fileName, cmd);
    cmd = bP;
  }
  //return redir_num;
}


 

 
