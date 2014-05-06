#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include "handler.h"
#include "errorcheck.h"
#include "global.h"

void sigchld_handler (int sig) {
    assert(sig == SIGCHLD);
    int status;
    pid_t child;
    while ((child = waitpid(-1, &status, WNOHANG)) > 0) {
        /*...do something with exited child's status */
	 if(child == -1){
	    int errnum = errno;
	    perror("Child process terminated with errors");
	    writeToLogFile(lineBCKP, "Child process terminated with errors", errnum);
	    exit(EXIT_FAILURE);
	  }
    }
}

/*
int
main(int argc, char *argv[])
{
    pid_t cpid, w;
    int status;

    cpid = fork();
    if (cpid == -1) {
	int errnum = errno;
	perror("fork");
	writeToLogFile(lineBCKP, "fork", errnum);
	exit(EXIT_FAILURE);
    }

    if (cpid == 0) {            /* Code executed by child *//*
	printf("Child PID is %ld\n", (long) getpid());
	if (argc == 1)
	    pause();                    /* Wait for signals *//*
	_exit(atoi(argv[1]));

    } else {                    /* Code executed by parent *//*
	do {
	    w = waitpid(cpid, &status, WUNTRACED | WCONTINUED);
	    if (w == -1) {
		perror("waitpid");
		exit(EXIT_FAILURE);
	    }

	    if (WIFEXITED(status)) {
		printf("exited, status=%d\n", WEXITSTATUS(status));
	    } else if (WIFSIGNALED(status)) {
		printf("killed by signal %d\n", WTERMSIG(status));
	    } else if (WIFSTOPPED(status)) {
		printf("stopped by signal %d\n", WSTOPSIG(status));
	    } else if (WIFCONTINUED(status)) {
		printf("continued\n");
	    }
	} while (!WIFEXITED(status) && !WIFSIGNALED(status));
	exit(EXIT_SUCCESS);
    }
}
*/

