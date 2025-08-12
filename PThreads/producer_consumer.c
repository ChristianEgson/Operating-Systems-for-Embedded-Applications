#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t not_empty = PTHREAD_COND_INITIALIZER;
static pthread_cond_t not_full = PTHREAD_COND_INITIALIZER;

static int units_available = 0;

static void* thread_produce(void *args)
{
    int ret;
    int units_produced = 0;
    units_produced = rand() % 20 + 1; //randomly generate a value 1-20
    printf("%d Units Ordered\n", units_produced);
    
    ret = pthread_mutex_lock(&mtx);
    if (ret != 0) return NULL;

    while (units_produced > (100 - units_available))
    {
        printf("Not enough space to produce %d units\n", units_produced);
        ret = pthread_cond_wait(&not_full, &mtx); 
        /* original did not check ret here */
    }
        
    units_available += units_produced; //add the random value to the total units available
    printf("%d Units Produced\n", units_produced);
    pthread_cond_signal(&not_empty);
    printf("There are %d Units Available\n", units_available);

    ret = pthread_mutex_unlock(&mtx);
    if (ret != 0) return NULL;

    /* sleep OUTSIDE the critical section so we don't block the other thread */
    usleep(1000 * 200);
        
    printf("Now Exiting Producer Thread...\n");
    return NULL;
}

static void* thread_consume(void *args)
{
    int ret; //used to verify success of pthread functions
    int units_consumed = 0;
    units_consumed = rand() % 5 + 1; //randomly generate value 1-5
    printf("%d Units Are To Be Consumed\n", units_consumed);

    //lock the critical section for consumption
    ret = pthread_mutex_lock(&mtx);
    if (ret != 0) return NULL;

    //if there are not enough units to consume, production operations will first take place
    while (units_consumed > units_available)
    {
        printf("There are no available units!\n");
        ret = pthread_cond_wait(&not_empty, &mtx); //waits for the supply of units to be not empty or greater than the # to be consumed
        if (ret != 0) return NULL;
    }

    units_available -= units_consumed; //updates the units available after consumption
    printf("%d Units Have Now Been Consumed\n", units_consumed);
    printf("There are now %d Units Available\n", units_available);
    
    pthread_cond_signal(&not_full); //signals the production function that the buffer is no longer full

    ret = pthread_mutex_unlock(&mtx); //unlocks the critical section to be accessed by another function
    if (ret != 0) return NULL;

    /* sleep OUTSIDE the critical section so we don't block the other thread */
    usleep(1000 * 200);

    printf("Consumer thread is now exiting...\n");
    return NULL;
}

int main(int argc, char*argv[])
{
    pthread_t t1, t2;
    int loops;
    int ret;
        
    srand (time(NULL));

    loops = (argc > 1) ? atoi(argv[1]) : 10;

    printf("Units Available: %d\n", units_available);

    for (int i = 0; i <= loops; i++){
        ret = pthread_create(&t1, NULL, thread_consume, NULL);
        if (ret != 0){
            fprintf(stderr, "Consumer Thread Was Not Created\n");
            exit(EXIT_FAILURE);
        }
        ret = pthread_create(&t2, NULL, thread_produce, NULL);
        if (ret != 0) {
            fprintf(stderr, "Failed to create producer thread\n");
            exit(EXIT_FAILURE);
        }

        /* Removed sleeps in main so we don't serialize thread completion artificially */
        pthread_join(t1, NULL);
        pthread_join(t2, NULL);
    }
    return 0;
}
