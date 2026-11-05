
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "minios.h"

void ipc_demo(void)
{
    int fd[2];
    pid_t pid;

    char job[100];
    char result[256];
    char buffer[256];

    printf("\n========== IPC WORKER DEMO ==========\n");

    /* Get job name from user */

    printf("Enter job name: ");

    if (fgets(job, sizeof(job), stdin) == NULL)
    {
        printf("Could not read job name.\n");
        return;
    }

    job[strcspn(job, "\n")] = '\0';

    if (job[0] == '\0')
    {
        printf("Job name cannot be empty.\n");
        return;
    }

    /* Create pipe */

    if (pipe(fd) == -1)
    {
        perror("pipe");
        return;
    }

    /* Create worker process */

    pid = fork();

    if (pid < 0)
    {
        perror("fork");
        close(fd[0]);
        close(fd[1]);
        return;
    }

    /* Child process: Worker */

    if (pid == 0)
    {
        close(fd[0]);

        snprintf(
            result,
            sizeof(result),
            "Worker PID %d processed job: %s",
            getpid(),
            job
        );

        if (write(fd[1], result, strlen(result) + 1) < 0)
        {
            perror("write");
            close(fd[1]);
            _exit(1);
        }

        close(fd[1]);

        _exit(0);
    }

    /* Parent process: MiniOS */

    else
    {
        close(fd[1]);

        printf("\nParent PID: %d\n", getpid());
        printf("Worker PID: %d\n", pid);

        ssize_t n = read(fd[0], buffer, sizeof(buffer));

        if (n > 0)
        {
            buffer[n - 1] = '\0';

            printf("\n========== JOB RESULT ==========\n");
            printf("%s\n", buffer);
            printf("Communication: Worker -> Pipe -> MiniOS\n");
            printf("================================\n");
        }
        else if (n < 0)
        {
            perror("read");
        }

        close(fd[0]);

        waitpid(pid, NULL, 0);
    }
}

