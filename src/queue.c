#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#include "minios.h"
#define QUEUE_SIZE 5
/*
 * Circular buffer.
 */
static int buffer[QUEUE_SIZE];

static int in = 0;
static int out = 0;

/*
 * Semaphores.
 *
 * empty_slots:
 * number of empty spaces.
 *
 * full_slots:
 * number of produced items.
 *
 * buffer_mutex:
 * protects the shared buffer.
 */
static sem_t empty_slots;
static sem_t full_slots;
static sem_t buffer_mutex;

static void *producer(void *arg)
{
    (void)arg;

    for (int item = 1; item <= 10; item++)
    {
        /*
         * Wait until there is space.
         */
        sem_wait(&empty_slots);

        /*
         * Enter critical section.
         */
        sem_wait(&buffer_mutex);

        buffer[in] = item;

        printf(
            "[Producer] Produced %d at slot %d\n",
            item,
            in
        );

        in = (in + 1) % QUEUE_SIZE;

        /*
         * Leave critical section.
         */
        sem_post(&buffer_mutex);

        /*
         * One more item is available.
         */
        sem_post(&full_slots);

        usleep(30000);
    }

    return NULL;
}

static void *consumer(void *arg)
{
    (void)arg;

    for (int i = 0; i < 10; i++)
    {
        /*
         * Wait until an item exists.
         */
        sem_wait(&full_slots);

        /*
         * Enter critical section.
         */
        sem_wait(&buffer_mutex);

        int item = buffer[out];

        printf(
            "[Consumer] Consumed %d from slot %d\n",
            item,
            out
        );

        out = (out + 1) % QUEUE_SIZE;

        /*
         * Leave critical section.
         */
        sem_post(&buffer_mutex);

        /*
         * One more empty slot is available.
         */
        sem_post(&empty_slots);

        usleep(50000);
    }

    return NULL;
}

void producer_consumer_demo(void)
{
    pthread_t producer_thread;
    pthread_t consumer_thread;

    in = 0;
    out = 0;

    /*
     * Initially all queue slots are empty.
     */
    sem_init(
        &empty_slots,
        0,
        QUEUE_SIZE
    );

    /*
     * Initially no items exist.
     */
    sem_init(
        &full_slots,
        0,
        0
    );

    /*
     * Only one thread can modify
     * the queue at a time.
     */
    sem_init(
        &buffer_mutex,
        0,
        1
    );

    printf("\n");
    printf("============= PRODUCER-CONSUMER =============\n");

    printf(
        "Buffer size = %d\n",
        QUEUE_SIZE
    );

    pthread_create(
        &producer_thread,
        NULL,
        producer,
        NULL
    );

    pthread_create(
        &consumer_thread,
        NULL,
        consumer,
        NULL
    );

    pthread_join(
        producer_thread,
        NULL
    );

    pthread_join(
        consumer_thread,
        NULL
    );

    printf(
        "Producer and consumer completed safely.\n"
    );

    sem_destroy(&empty_slots);
    sem_destroy(&full_slots);
    sem_destroy(&buffer_mutex);

    printf(
        "==============================================\n\n"
    );
}
