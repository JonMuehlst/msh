#ifndef CONSTANTS_H_   /* Include guard */
#define CONSTANTS_H_

#define EXIT "exit"
#define EXITN "exit\n"

#define PIPE "|"
#define AMPERSAND "&"
#define AND "&&"
#define NEWLINE "\n"
#define DELIMITERS "|&"

#define OUTPUT_REDIRECTION ">"
#define APPEND_OUTPUT_REDIRECTION ">>"
#define INPUT_REDIRECTION "<"

#define OUTPUT_REDIRECTION_NUM 210
#define APPEND_OUTPUT_REDIRECTION_NUM 211
#define INPUT_REDIRECTION_NUM 2112

#define STANDARD_INPUT 0
#define STANDARD_OUTPUT 1
#define STANDARD_ERROR 2

#define FALSE 0
#define TRUE 1

#define PIPE_READ  0
#define PIPE_WRITE 1
 
#define NO_BLOCK_ENDING 110
#define PIPE_BLOCK_ENDING 111
#define AMPERSAND_BLOCK_ENDING 112
#define AND_BLOCK_ENDING 113

#define NEWLINE_NUM 310
#define PIPE_NUM 311
#define AMPERSAND_NUM 312
#define NULL_NUM 313

#endif 
