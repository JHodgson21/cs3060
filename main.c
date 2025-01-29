#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    printf("Assignment 2 by Jakob Hodgson\n");

    // Forking
    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    } else if (pid == 0) {
        // CHILD process begins
        printf("CHILD has started. ");
        if (argc > 1) {
            if (argc == 2) {
                printf("One argument was provided. Calling execlp(), never to return (sniff)\n");
                execlp(argv[1], argv[1], NULL);
            } else {
                printf("More than one argument was provided. Calling execvp(), never to return (sniff)\n");
                execvp(argv[1], &argv[1]);
            }
            //if the exec fails we go here
            perror("Exec failed");
            exit(1);
        } else {
            printf("No arguments provided. Terminating child.\n");
            exit(0);
        }
    } else {
        //PARENT process begins
        printf("PARENT has started, now waiting for process ID #%d\n", pid);
        int status;
        waitpid(pid, &status, 0);
        printf("PARENT has now resumed. Child exit code of %d. Now terminating parent.\n", WEXITSTATUS(status));
    }

    return 0;
}

