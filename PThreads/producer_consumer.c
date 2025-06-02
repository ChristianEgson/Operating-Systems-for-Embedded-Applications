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
    units_produced = rand() % 20 + 1; //randomly generate a value 0-20
    pthread_t thread_id = pthread_self();

    printf("[P:%lu] %d Units Ordered\n", (unsigned long)thread_id, units_produced);
    
    ret = pthread_mutex_lock(&mtx);
    if (ret != 0) return NULL;

    while (units_produced > (100 - units_available))
    {
        printf("[P:%lu] Not enough space to produce %d units\n", (unsigned long)thread_id, units_produced);
        ret = pthread_cond_wait(&not_full, &mtx); 
    }
        
    units_available += units_produced; //add the random value to the total units available
    printf("[P:%lu] %d Units Produced | There are now %d units available\n", (unsigned long)thread_id, units_produced, units_available);
    pthread_cond_signal(&not_empty);        

    ret = pthread_mutex_unlock(&mtx);
    if (ret != 0) return NULL;

    printf("[P:%lu] Now Exiting Producer Thread...\n", (unsigned long)thread_id);
    return NULL;
}

static void* thread_consume(void *args)
{
    int ret; //used to verify success of pthread functions
    int units_consumed = 0;
    units_consumed = rand() % 5 + 1; //randomly generate value 1-5
    pthread_t thread_id = pthread_self();

    printf("[C:%lu] %d Units Are To Be Consumed\n", (unsigned long)thread_id, units_consumed);

    //lock the critical section for consumption
    ret = pthread_mutex_lock(&mtx);
    if (ret != 0) return NULL;

    //if there are not enough units to consume, production operations will first take place
    while (units_consumed > units_available)
    {
        printf("[C:%lu] There are no available units!\n", (unsigned long)thread_id);
        ret = pthread_cond_wait(&not_empty, &mtx); //waits for the supply of units to be not empty or greater than the # to be consumed
        if (ret != 0) return NULL;
    }

    units_available -= units_consumed; //updates the units available after consumption
    printf("[C:%lu] %d Units Have Now Been Consumed | There are now %d units available\n", (unsigned long)thread_id, units_consumed, units_available);
    
    pthread_cond_signal(&not_full); //signals the production function that the buffer is no longer full
    ret = pthread_mutex_unlock(&mtx); //unlocks the critical section to be accessed by another function
    if (ret != 0) return NULL;

    printf("[C:%lu] Consumer thread is now exiting...\n", (unsigned long)thread_id);
    return NULL;
}

int main(int argc, char*argv[])
{
    pthread_t t1, t2;
    int loops;
    int ret;
        
    srand (time(NULL)); //ensures that a random value is produced when using rand()

    loops = (argc > 1) ? atoi(argv[1]) : 10; //terminal input when running the program determines the # of loops

    printf("Units Available: %d\n", units_available);

    //creates and verifies the creation of the consumer threads
    for (int i = 0; i <= loops; i++){
        ret = pthread_create(&t1, NULL, thread_consume, NULL);
        if (ret != 0){
            fprintf(stderr, "Consumer Thread Was Not Created\n");
            exit(EXIT_FAILURE);
        }

        //detaches (thread cleans up resouces and exits on own) the consumer threads and verifies its success 
        ret = pthread_detach(t1);
        if (ret != 0){
            fprintf(stderr, "Consumer thread failed to detach\n");
            exit(EXIT_FAILURE);
        }

        usleep(1000000 * 2);

        //creates and verifies the creation of the producer threads
        ret = pthread_create(&t2, NULL, thread_produce, NULL);
        if (ret != 0) {
            fprintf(stderr, "Failed to create producer thread\n");
            exit(EXIT_FAILURE);
        }

        //detaches (thread cleans up resouces and exits on own) the consumer threads and verifies its success 
        ret = pthread_detach(t2);
        if (ret != 0){
            fprintf(stderr, "Producer thread failed to detach\n");
            exit(EXIT_FAILURE);
        }

        usleep(1000000 * 2);
        
    }
    return 0;
}


