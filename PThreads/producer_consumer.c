#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <string.h> // For strerror, if needed for pthread errors

// --- Configuration ---
#define MAX_BUFFER_SIZE 100 // Implicit buffer size from original producer logic

// Define desired creation intervals (these can be changed)
// Interval for creating new PRODUCER threads
#define PRODUCER_CREATION_INTERVAL_US (2 * 1000000) // e.g., every 2 seconds
// Interval for creating new CONSUMER threads
#define CONSUMER_CREATION_INTERVAL_US (1 * 1000000) // e.g., every 1 second


// --- Global Shared Resources (from original code) ---
static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t not_empty = PTHREAD_COND_INITIALIZER;
static pthread_cond_t not_full = PTHREAD_COND_INITIALIZER;
static int units_available = 0;

// --- Producer Thread Function (Identical to your original) ---
static void* thread_produce(void *args)
{
    int ret;
    int units_produced = 0;
    units_produced = rand() % 20 + 1; //randomly generate a value 0-20
    pthread_t thread_id = pthread_self();

    printf("[P:%lu] %d Units Ordered\n", (unsigned long)thread_id, units_produced);
    
    ret = pthread_mutex_lock(&mtx);
    if (ret != 0) {
        fprintf(stderr, "[P:%lu] Mutex lock failed: %s\n", (unsigned long)thread_id, strerror(ret));
        return NULL;
    }

    while (units_produced > (MAX_BUFFER_SIZE - units_available))
    {
        printf("[P:%lu] Not enough space to produce %d units (Available space: %d). Waiting...\n", (unsigned long)thread_id, units_produced, MAX_BUFFER_SIZE - units_available);
        ret = pthread_cond_wait(&not_full, &mtx);
        if (ret != 0) {
            fprintf(stderr, "[P:%lu] Cond wait failed: %s\n", (unsigned long)thread_id, strerror(ret));
            pthread_mutex_unlock(&mtx);
            return NULL;
        }
    }
        
    units_available += units_produced; //add the random value to the total units available
    printf("[P:%lu] %d Units Produced | There are now %d units available (Buffer: %d/%d)\n", (unsigned long)thread_id, units_produced, units_available, units_available, MAX_BUFFER_SIZE);
    pthread_cond_signal(&not_empty);        

    ret = pthread_mutex_unlock(&mtx);
    if (ret != 0) {
        fprintf(stderr, "[P:%lu] Mutex unlock failed: %s\n", (unsigned long)thread_id, strerror(ret));
        return NULL;
    }

    printf("[P:%lu] Now Exiting Producer Thread...\n", (unsigned long)thread_id);
    return NULL;
}

// --- Consumer Thread Function (Identical to your original) ---
static void* thread_consume(void *args)
{
    int ret; //used to verify success of pthread functions
    int units_consumed = 0;
    units_consumed = rand() % 5 + 1; //randomly generate value 1-5
    pthread_t thread_id = pthread_self();

    printf("[C:%lu] %d Units Are To Be Consumed\n", (unsigned long)thread_id, units_consumed);

    //lock the critical section for consumption
    ret = pthread_mutex_lock(&mtx);
    if (ret != 0) {
        fprintf(stderr, "[C:%lu] Mutex lock failed: %s\n", (unsigned long)thread_id, strerror(ret));
        return NULL;
    }

    //if there are not enough units to consume, production operations will first take place
    while (units_consumed > units_available)
    {
        printf("[C:%lu] There are not enough units (%d available for %d requested)! Waiting...\n", (unsigned long)thread_id, units_available, units_consumed);
        ret = pthread_cond_wait(&not_empty, &mtx); //waits for the supply of units to be not empty or greater than the # to be consumed
        if (ret != 0) {
            fprintf(stderr, "[C:%lu] Cond wait failed: %s\n", (unsigned long)thread_id, strerror(ret));
            pthread_mutex_unlock(&mtx);
            return NULL;
        }
    }

    units_available -= units_consumed; //updates the units available after consumption
    printf("[C:%lu] %d Units Have Now Been Consumed | There are now %d units available (Buffer: %d/%d)\n", (unsigned long)thread_id, units_consumed, units_available, units_available, MAX_BUFFER_SIZE);
    
    pthread_cond_signal(&not_full); //signals the production function that the buffer is no longer full
    ret = pthread_mutex_unlock(&mtx); //unlocks the critical section to be accessed by another function
    if (ret != 0) {
        fprintf(stderr, "[C:%lu] Mutex unlock failed: %s\n", (unsigned long)thread_id, strerror(ret));
        return NULL;
    }

    printf("[C:%lu] Consumer thread is now exiting...\n", (unsigned long)thread_id);
    return NULL;
}

// --- Main Function (Minimally Modified) ---
int main(int argc, char*argv[])
{
    pthread_t tid; // Using a single thread ID variable, as loops are separate
    int loops;
    int ret;
        
    srand (time(NULL)); // Ensures that a random value is produced when using rand()

    loops = (argc > 1) ? atoi(argv[1]) : 10; // Terminal input determines # of loops for each type

    printf("Initial Units Available: %d/%d\n", units_available, MAX_BUFFER_SIZE);
    printf("Main: Will create %d consumer threads and %d producer threads.\n", loops + 1, loops + 1);

    // Create consumer threads
    printf("--- Creating %d Consumer Threads (approx. every %.1f seconds) ---\n", loops + 1, CONSUMER_CREATION_INTERVAL_US / 1000000.0);
    for (int i = 0; i <= loops; i++) { // Create 'loops + 1' consumer threads
        ret = pthread_create(&tid, NULL, thread_consume, NULL);
        if (ret != 0) {
            fprintf(stderr, "Consumer Thread #%d Was Not Created: %s\n", i + 1, strerror(ret));
            exit(EXIT_FAILURE); // Original code exits on failure
        }
        // printf("Main: Created consumer thread #%d (ID: %lu)\n", i + 1, (unsigned long)tid);

        ret = pthread_detach(tid);
        if (ret != 0) {
            fprintf(stderr, "Consumer thread #%d failed to detach: %s\n", i + 1, strerror(ret));
            exit(EXIT_FAILURE); // Original code exits on failure
        }

        // Sleep for the defined interval, but not after creating the last consumer in this block
        if (i < loops) {
            usleep(CONSUMER_CREATION_INTERVAL_US);
        }
    }

    // Create producer threads
    printf("--- Creating %d Producer Threads (approx. every %.1f seconds) ---\n", loops + 1, PRODUCER_CREATION_INTERVAL_US / 1000000.0);
    for (int i = 0; i <= loops; i++) { // Create 'loops + 1' producer threads
        ret = pthread_create(&tid, NULL, thread_produce, NULL);
        if (ret != 0) {
            fprintf(stderr, "Producer Thread #%d Was Not Created: %s\n", i + 1, strerror(ret));
            exit(EXIT_FAILURE); // Original code exits on failure
        }
        // printf("Main: Created producer thread #%d (ID: %lu)\n", i + 1, (unsigned long)tid);

        ret = pthread_detach(tid);
        if (ret != 0) {
            fprintf(stderr, "Producer thread #%d failed to detach: %s\n", i + 1, strerror(ret));
            exit(EXIT_FAILURE); // Original code exits on failure
        }

        // Sleep for the defined interval, but not after creating the last producer in this block
        if (i < loops) {
            usleep(PRODUCER_CREATION_INTERVAL_US);
        }
    }
    
    printf("Main: All %d consumer and %d producer threads have been created.\n", loops + 1, loops + 1);
    printf("Main: Allowing worker threads some time to run before exiting (e.g., 10 seconds)...\n");
    
    // Give detached threads some time to complete their work.
    // A more robust system might involve joining threads or other synchronization.
    sleep(15); // Increased sleep duration to allow more operations

    printf("Main: Exiting. Final Units Available: %d/%d\n", units_available, MAX_BUFFER_SIZE);
    
    pthread_mutex_destroy(&mtx);
    pthread_cond_destroy(&not_empty);
    pthread_cond_destroy(&not_full);

    return 0;
}
