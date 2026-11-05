#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>

#include "minios.h"

/* Shared counter used by both threads. */
static long safe_count = 0;

/* Semaphore used to protect the shared counter. */
static sem_t mutex_sem;

/* Function executed by each thread. */
static void *safe_worker(void *arg)
{
    (void)arg;

    for (long i = 0; i < 10000; i++)
    {
        /* Wait until the semaphore allows entry. */
        while (sem_wait(&mutex_sem) == -1)
        {
            if (errno == EINTR)
                continue;

            perror("sem_wait failed");
            return NULL;
        }

        /* Critical section: safely update shared data. */
        safe_count++;

        /* Release the semaphore. */
        if (sem_post(&mutex_sem) == -1)
        {
            perror("sem_post failed");
            return NULL;
        }
    }

    return NULL;
}

/* Demonstrate synchronization using a semaphore. */
void semaphore_demo(void)
{
    pthread_t thread1;
    pthread_t thread2;
    int created = 0;

    safe_count = 0;

    /* Initialize semaphore with value 1. */
    if (sem_init(&mutex_sem, 0, 1) == -1)
    {
        perror("sem_init failed");
        return;
    }

    printf("\n");
    printf("============= SYNCHRONIZATION =============\n");
    printf("Two threads each perform 10,000 increments.\n");
    printf("Expected result = 20,000\n");

    /* Create the first thread. */
    int result = pthread_create(
        &thread1, NULL, safe_worker, NULL
    );

    if (result != 0)
    {
        printf("Could not create thread 1. Error: %d\n", result);
        sem_destroy(&mutex_sem);
        return;
    }

    created++;

    /* Create the second thread. */
    result = pthread_create(
        &thread2, NULL, safe_worker, NULL
    );

    if (result != 0)
    {
        printf("Could not create thread 2. Error: %d\n", result);
    }
    else
    {
        created++;
    }

    /* Wait for every successfully created thread. */
    if (created >= 1)
    {
        result = pthread_join(thread1, NULL);

        if (result != 0)
            printf("Could not join thread 1. Error: %d\n", result);
    }

    if (created == 2)
    {
        result = pthread_join(thread2, NULL);

        if (result != 0)
            printf("Could not join thread 2. Error: %d\n", result);
    }

    printf("Actual result = %ld\n", safe_count);
    printf("Semaphore protected the critical section.\n");

    /* Destroy the semaphore after both threads finish. */
    if (sem_destroy(&mutex_sem) == -1)
        perror("sem_destroy failed");

    printf("===========================================\n\n");
}
