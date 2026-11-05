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

void show_race_demo(void)
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
