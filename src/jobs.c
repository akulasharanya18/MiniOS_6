#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#include "minios.h"

static void *worker_function(void *arg)
{
    int id = *(int *)arg;

    printf(
        "[Worker %d] Started.\n",
        id
    );

    usleep(100000);

    printf(
        "[Worker %d] Completed its job.\n",
        id
    );

    return NULL;
}

void show_workers_demo(void)
{
    pthread_t threads[WORKER_COUNT];

    int ids[WORKER_COUNT];

    printf("\n");
    printf("============= WORKER MANAGER =============\n");

    for (int i = 0; i < WORKER_COUNT; i++)
    {
        ids[i] = i + 1;

        if (pthread_create(
                &threads[i],
                NULL,
                worker_function,
                &ids[i]) != 0)
        {
            perror("pthread_create");
            return;
        }
    }

    for (int i = 0; i < WORKER_COUNT; i++)
    {
        pthread_join(
            threads[i],
            NULL
        );
    }

    printf(
        "All worker threads completed.\n"
    );

    printf(
        "===========================================\n\n"
    );
}
