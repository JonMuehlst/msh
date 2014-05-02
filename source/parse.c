/*
 * parse.c
 */

#include "parse.h"

struct cmdBlock { // this is a command block
    int argc;			/* Number of arguments */
    char *argv[MAXARGS];	/* The arguments list */
    char *infile;		/* The input file */
    char *outfile;		/* The output file */
    int redirectionFlag;	/* Specify if an io redirection is taking place */
    int blockEndingFlag;	/* Specify the block ending */
}; 

  /* check input for errors. */
  /* on valid input, the function returns the number of command blocks */
  /* on invalid input the function returns -1*/
  int chck_general_error(char *line){
    int inputRedirFlag;		/* Specify if an input redirection is taking place */
    int outputRedirFlag;	/* Specify if an output redirection is taking place */
    int pipeFlag;		/* Specify if the block is ended by a pipe */
    int ampersandFlag;		/* Specify if the block is ended by an ampersand */
    int andFlag;		/* Specify if the block is ended by a double ampersand */
   //
   //
   //
   //
  }

  void init_block_struct(cmdBlock *blck, int ) {
      blck->argc = 0;
      blck->
      blck->
      blck->
      blck->
      blck->
  }

  /* parse a single command */
  void parse_command_block(char * command, struct commandType *comm) {
	
  }

  /*   parse commandline for space separated commands */
  cmdBlock *parse (char *cmdline) {
    foreach cmd in cmdline {
      if (cmd == command) {
        parse_command(cmd, type)
      }
    }
  }

  /* prints out parse struct */
  void print_info (cmdBlock *info) {
     foreach type in cmdBlock {
       print "type_name: type"
     }
  }  

  /*  free memory used in cmdBlock */
  void free_info (cmdBlock *info) {
     foreach memory_block in cmdBlock
        free(memory_block)
  }
 
void build_block(char * line, cmdBlock * blck){
  char * string = (char *)malloc(1024*sizeof(char));
  string = strtok(line, " ");
  while(string != NULL){
      if(strcmp(string, PIPE) == 0){
	blck->pipeFlag = TRUE;
      } else if(strcmp(string, AMPERSAND) == 0){
	blck->ampersandFlag = TRUE;
      } else if(strcmp(string, AND) == 0 ){
	blck->andFlag = TRUE;
      }
    
    
  }
  free(string);
}

//first find amount of delimiters and set commandBlock array size

int number_of_tokens(char * string){
  char str[1024];
  char key[] = "|&";
  char * pch;
  int count = 0;
  
  //printf ("Delimiters in '%s': ",str);
  pch = strpbrk (str, key);
  while (pch != NULL)
  {
    count++;
    //printf ("%c " , *pch);
    pch = strpbrk (pch+1,key);
  }
  //printf ("\n");
  //printf ("%s\n", str);
  return count + 1;
}

int number_of_words_and_symbols(char* str)
{
  int counter = 0;
  char * pch;
  
  pch = strtok (str," ");
  while (pch != NULL)
  {
    if(strcmp(pch, "\n"))
      counter++;
    pch = strtok (NULL, " ");
  }
  
  return counter;
}
