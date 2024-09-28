#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


int main(int argc, char **argv) {

    char* arguments[argc+1];
    for (int i=0; i<argc; ++i){
        arguments[i] = argv[i];
    }
    arguments[argc] = NULL;

    pid_t pid = fork();

    if ( pid == 0 ) {
        execv( "./sequential_min_max", arguments );
    }
    int status;
    waitpid(pid, &status, 0);
    if (!WIFEXITED(status)) {
      printf("Child process %d did not exit normally\n", pid);
    }

    return 0;
}
