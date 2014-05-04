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
int command_pipe[2];
 
/*
 * Handle commands separatly
 * input: return value from previous command (useful for pipe file descriptor)
 * first: 1 if first command in pipe-sequence (no input from previous pipe)
 * last: 1 if last command in pipe-sequence (no input from previous pipe)
 *
 * EXAMPLE: If you type "ls | grep shell | wc" in your shell:
 *    fd1 = command(0, 1, 0), with args[0] = "ls"
 *    fd2 = command(fd1, 0, 0), with args[0] = "grep" and args[1] = "shell"
 *    fd3 = command(fd2, 0, 1), with args[0] = "wc"
 *
 * So if 'command' returns a file descriptor, the next 'command' has this
 * descriptor as its 'input'.
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
	
	return STANDARD_INPUT;
}
*/
 
int run(char* cmd, int input, int first, int last, int* n){
	split(cmd, args);
	if (args[0] != NULL) { /*
		if (strcmp(args[0], EXIT) == 0) 
			exit(0); */
		(*n) += 1;
		return pipeCommand(input, first, last);
	}
	return 0;
}
 

 
