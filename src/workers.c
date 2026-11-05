#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#include "minios.h"

/*
 * Shared variable.
 *
 * Two threads access this variable
 * without synchronization.
 */
static volatile long race_count = 0;

static void *race_worker(void *arg)
{
    (void)arg;

    for (long i = 0; i < 10000; i++)
    {
        /*
         * Read
         */
        long temp = race_count;

        /*
         * Small delay makes the race
         * easier to observe.
         */
        usleep(1);

        /*
         * Write
         */
        race_count = temp + 1;
    }

    return NULL;
}

/* Function executed by each worker thread. */
static void *worker_task(void *arg)
{
    int worker_id = *(int *)arg;

    printf("[Worker %d] Started.\n", worker_id);

    usleep(100000);

    printf("[Worker %d] Completed its job.\n", worker_id);

    return NULL;
}

/* Create and manage three worker threads. */
void start_workers(void)
{
    pthread_t threads[3];
    int worker_ids[3] = {1, 2, 3};
    int created = 0;

    printf("\n");
    printf("========== WORKER MANAGER ==========\n");

    /* Create three worker threads. */
    for (int i = 0; i < 3; i++)
    {
        int result = pthread_create(
            &threads[i],
            NULL,
            worker_task,
            &worker_ids[i]
        );

        if (result != 0)
        {
            printf("Could not create worker %d. Error: %d\n",
                   worker_ids[i], result);
            break;
        }

        created++;
    }

    /* Wait for successfully created threads. */
    for (int i = 0; i < created; i++)
    {
        int result = pthread_join(threads[i], NULL);

        if (result != 0)
        {
            printf("Could not join worker %d. Error: %d\n",
                   worker_ids[i], result);
        }
    }

    printf("All created worker threads have finished.\n");
    printf("====================================\n");
}

void race_demo(void)
{
    pthread_t thread1;
    pthread_t thread2;

    race_count = 0;

    printf("\n");
    printf("=============== RACE DEMO ===============\n");

    printf(
        "Thread 1: 10,000 increments\n"
    );

    printf(
        "Thread 2: 10,000 increments\n"
    );

    printf(
        "Expected result = 20,000\n"
    );

    pthread_create(
        &thread1,
        NULL,
        race_worker,
        NULL
    );

    pthread_create(
        &thread2,
        NULL,
        race_worker,
        NULL
    );

    pthread_join(
        thread1,
        NULL
    );

    pthread_join(
        thread2,
        NULL
    );

    printf(
        "Actual result   = %ld\n",
        race_count
    );

    printf(
        "Lost updates demonstrate a race condition.\n"
    );

    printf(
        "==========================================\n\n"
    );
}
