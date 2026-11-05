#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "minios.h"

int run_program(char **args)
{
    pid_t pid;

    pid = fork();

    if (pid < 0)
    {
        perror("fork");
        return -1;
    }

    if (pid == 0)
    {
        printf(
            "[Child] PID = %d\n",
            getpid()
        );

        printf(
            "[Child] Executing program: %s\n",
            args[0]
        );

        fflush(stdout);

        execvp(args[0], args);

        perror("[Child] execvp failed");

        _exit(127);
    }

    printf(
        "[Parent] Created child PID = %d\n",
        pid
    );

    printf("[Parent] Waiting for child...\n");

    int status;

    if (waitpid(pid, &status, 0) < 0)
    {
        perror("waitpid");
        return -1;
    }

    if (WIFEXITED(status))
    {
        printf(
            "[Parent] Child exited with status %d.\n\n",
            WEXITSTATUS(status)
        );
    }
    else
    {
        printf(
            "[Parent] Child did not exit normally.\n\n"
        );
    }

    return 0;
}


void process_demo(void)
{
    char *args[] = {"ls", "-l", NULL};

    printf("\n================================\n");
    printf("       PROCESS MANAGEMENT\n");
    printf("================================\n");

    printf("\nStarting process demonstration...\n");

    if (run_program(args) == 0) {
        printf("\nProcess demo completed successfully!\n");
    } else {
        printf("\nProcess demo failed.\n");
    }
}