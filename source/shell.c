#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "constants.h"

/*	Functions
 * 
 * 
 */


int main(int argc, char *argv[]){
  
  char line[1024];
  char lineBCKP[1024];
  while(1) {
    printf("$ ");
    if(!fgets(line, 1024, stdin)){
      printf("\n");
      break;
    }
    if(strcmp(EXITN,line) == 0){
      break;
    }
    memcpy ( lineBCKP, line, strlen(line)+1 );
    
    //call to parser here
    //info  = parse(cmdLine);

    //call to executer here
    
    //print_info(info);
    
  }
  
  //free_info(info);

} 


    