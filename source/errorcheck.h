#ifndef ERRORCHECK_H_   /* Include guard */
#define ERRORCHECK_H_

#include "constants.h"

    int check_consec_delim();  
    int check_outputredir_before_delim();
    int check_outputappendredir_before_delim();
    int check_emptyredir_before_delim1();
    int check_emptyredir_before_delim2();
    int check_emptyredir_last();
    int check_more_than_one_redir_in_block();
    int check_qoutation_mark();
    int check_pipe_last();
    void check_all();

    int chck_general_error(char *line);
    void skipBlanks(char *srcPtr);



#endif 
