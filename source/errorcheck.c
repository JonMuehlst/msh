

// todo:
// * check if an expression is mixed i.e >>text (without whitespaces)

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "global.h"
#include "errorcheck.h"

  // < unit tests >
    //check consecutive delimiters
    int check_consec_delim(){
      char string1[] = "|&";
      char string2[] = "| &";
      if(chck_general_error(string1) == -1 || chck_general_error(string2) == -1)
	return 1;
      return 0;
    }
    
    int check_outputredir_before_delim(){
      char string1[] = "a > a |";
      if(chck_general_error(string1) == -1)
	return 1;
      return 0;
    }
    
    int check_outputappendredir_before_delim(){
      char string1[] = "a >> a |";
      if(chck_general_error(string1) == -1)
	return 1;
      return 0;
    }
    
    int check_emptyredir_before_delim1(){
      char string1[] = "a < &";
      if(chck_general_error(string1) == -1)
	return 1;
      return 0;
    }
    
    int check_emptyredir_before_delim2(){
      char string1[] = "a >> &";
      if(chck_general_error(string1) == -1)
	return 1;
      return 0;
    }
    
    int check_emptyredir_last(){
      char string1[] = "a < ";
      if(chck_general_error(string1) == -1)
	return 1;
      return 0;
    }
    
    int check_more_than_one_redir_in_block(){
      char string1[] = "a < app | app < file1 > file2 ";
      if(chck_general_error(string1) == -1)
	return 1;
      return 0;
    }
    
    int check_qoutation_mark(){
      char string1[] = "a bds \"";
      if(chck_general_error(string1) == -1)
	return 1;
      return 0;
    }
    
    int check_pipe_last(){
      char string1[] = "a bds |";
      if(chck_general_error(string1) == -1)
	return 1;
      return 0;
    }
    
    void check_all_errorcheck(){
      int flag = 1;
      if(!check_pipe_last()){
	printf("pipe_last_check_incorrect\n");
	flag = 0;
      }
      if(!check_qoutation_mark()){
	printf("qoutation_mark_check_incorrect\n");
	flag = 0;
      }
      if(!check_more_than_one_redir_in_block()){
	printf("more_than_one_redir_in_block_check_incorrect\n");
	flag = 0;
      }
      if(!check_emptyredir_last()){
	printf("empty_redir_last_check_incorrect\n");
	flag = 0;
      }
      if(!check_emptyredir_before_delim2()){
	printf("emptyredir_before_delim2_check_incorrect\n");
	flag = 0;
      }
      if(!check_emptyredir_before_delim1()){
	printf("emptyredir_before_delim1_check_incorrect\n");
	flag = 0;
      }
      if(!check_outputappendredir_before_delim()){
	printf("outputappendredir_before_delim_check_incorrect\n");
	flag = 0;
      }
      if(!check_outputredir_before_delim()){
	printf("outputredir_before_delim_check_incorrect\n");
	flag = 0;
      }
      if(!check_consec_delim()){
	printf("consec_delim_check_incorrect\n");
	flag = 0;
      }
      if(flag)
	printf("All tests are OK\n");
    }
  // / > unit tests
  
  /* check input for errors. */
  /* on valid input, the function returns the number of command blocks */
  /* on invalid input the function returns -1*/
  int chck_general_error(char *line){
    
    int inputRedirFlag = 0;	/* Specify if an input redirection is taking place */
    int outputRedirFlag = 0;	/* Specify if an output redirection is taking place */
    int pipeFlag = 0;		/* Specify if the block is ended by a pipe */
    int ampersandFlag = 0;	/* Specify if the block is ended by an ampersand */
    int delimiter = 0;		/* Specify whether a delimiter was found*/
    int redirFlag = 0;		/* Specifies empty redirection */
    int isTextFlag = 0;
    char* srcPtr;
    
    srcPtr = line;
    skipBlanks(&srcPtr);
    while(*srcPtr){ // assuming white spaces are always skipped
      
      printf(" %c", *srcPtr); // for debugging
      
      if(*srcPtr == '|' || *srcPtr == '&'){
	if(delimiter)
	    return -1;
	if(*srcPtr == '|' ){ //output redirection before a pipe is not allowed.
	  if(outputRedirFlag)
	    return -1;
	}
	if(redirFlag) // // empty redirection before delimiter
	    return -1;
	delimiter = 1;
	//no error in the last block. reinitialize flags.
	outputRedirFlag = 0; // should not be necessary. 
	redirFlag = 0;
	inputRedirFlag = 0;
      } else {
	delimiter = 0;
      }
      
      if(*srcPtr == '|'){
	pipeFlag = 1;
      } else {
	pipeFlag = 0;
      }
      
      if(*srcPtr == '\"'){// should not contain qoutation marks
	return -1;
      }
      
      if(*(srcPtr + 1)){
	if(*srcPtr == '>' && *(srcPtr + 1) == '>'){
	  if(redirFlag){
	    return -1;
	  }
	  redirFlag = 1;
	  outputRedirFlag = 1;
	  srcPtr += 2;
	  if(*srcPtr == ' '){
	    skipBlanks(&srcPtr);
	    break;
	  }
	}
      } 
      
      if(*srcPtr == '>' || *srcPtr == '<'){
	if(outputRedirFlag || inputRedirFlag){
	    return -1;
	}
	if(*srcPtr == '>'){
	  outputRedirFlag = 1;
	} else {
	  inputRedirFlag = 1;
	}
	redirFlag = 1;
      } else {
	redirFlag = 0;
      } 
      
      ++srcPtr;
      if(*srcPtr == ' '){
	skipBlanks(&srcPtr);
      }
    }
    
    if(pipeFlag){
      //if the pipeFlag is set after the loop, the command line is ended by one. 
      return -1;
    }
    
    if(redirFlag){
      return -1;
    }
  
      printf("\n"); //for debugging
 }

  void skipBlanks(char **srcPtr){
	  while((**srcPtr == ' ' || **srcPtr == '\t' || **srcPtr == '\n') && **srcPtr)
	  {
		  (*srcPtr)++;
	  }
	  
  }

  void writeToLogFile(const char* line, const char* str, int errnum){
    
    char tmp[1024]; 
    strcpy(tmp, line);
    char* next = strchr(tmp,'\n');
    *next = '\0';
    
    FILE* pFile = fopen("error.log", "a");
    fprintf(pFile, "<%s>: <%s, %s>\n",tmp, str, strerror( errnum ));
    fclose(pFile);
  
  }
