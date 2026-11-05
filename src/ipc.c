#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "minios.h"

void submit_job(const char *job)
{
    int fd[2];

    if (pipe(fd) == -1)
    {
        perror("pipe");
        return;
    }

    pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork");

        close(fd[0]);
        close(fd[1]);

        return;
    }

    if (pid == 0)
    {
        char result[256];

        /*
         * Child does not read.
         * Therefore close read end.
         */
        close(fd[0]);

        snprintf(
            result,
            sizeof(result),
            "Worker PID %d processed job: %s",
            getpid(),
            job
        );

        write(
            fd[1],
            result,
            strlen(result) + 1
        );

        close(fd[1]);

        _exit(0);
    }

    /*
     * Parent does not write.
     * Therefore close write end.
     */
    close(fd[1]);

    char buffer[256];

    ssize_t n = read(
        fd[0],
        buffer,
        sizeof(buffer) - 1
    );

    if (n > 0)
    {
        buffer[n] = '\0';

        printf("\n");
        printf("========== JOB RESULT ==========\n");

        printf("%s\n", buffer);

        printf(
            "Communication: worker -> pipe -> MiniOS\n"
        );

        printf("================================\n\n");
    }

    close(fd[0]);

    waitpid(pid, NULL, 0);
}
