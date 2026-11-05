#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#include "minios.h"

static long safe_count = 0;

static sem_t mutex_sem;

static void *safe_worker(void *arg)
{
    (void)arg;

    for (long i = 0; i < 10000; i++)
    {
        /*
         * Enter critical section.
         */
        sem_wait(&mutex_sem);

        safe_count++;

        /*
         * Leave critical section.
         */
        sem_post(&mutex_sem);
    }

    return NULL;
}

void show_sync_demo(void)
{
    pthread_t thread1;
    pthread_t thread2;

    safe_count = 0;

    /*
     * Initial value = 1.
     *
     * This makes the semaphore act
     * like a lock.
     */
    sem_init(
        &mutex_sem,
        0,
        1
    );

    printf("\n");
    printf("============= SYNCHRONIZATION =============\n");

    printf(
        "Two threads each perform 10,000 increments.\n"
    );

    printf(
        "Expected result = 20,000\n"
    );

    pthread_create(
        &thread1,
        NULL,
        safe_worker,
        NULL
    );

    pthread_create(
        &thread2,
        NULL,
        safe_worker,
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
        safe_count
    );

    printf(
        "Semaphore protected the critical section.\n"
    );

    sem_destroy(&mutex_sem);

    printf(
        "============================================\n\n"
    );
}
