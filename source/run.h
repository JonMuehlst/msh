#ifndef RUN_H_   /* Include guard */
#define RUN_H_

  int run(char* cmd, int input, int first, int last, int* n, int delimitFlag);
  int pipeCommand(int input, int first, int last);
  int bgCommand(int input, int first, int output);
  int command(int input, int first, int output);
  void getRedir(char* cmd_org);
  void print_run_check(char* fileName, int redir_num);
  void check_run();
  void openFile();
  void closeFile();


#endif 