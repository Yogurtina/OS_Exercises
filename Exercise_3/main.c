#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>

#define BUFFER_SIZE 10
#define MAXNUMBER 200

typedef struct {
    int num;
    char str[10];
} item;

// shared variables
item buffer[BUFFER_SIZE]; // the buffer
int in = 0; // save item
int out = 0; // position to read 

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t empty;
sem_t full;

// initialize buffer
void init_buffer() {
    for (int i = 0; i < BUFFER_SIZE; i++) {
        buffer[i].num = -1;
        sprintf(buffer[i].str, "%3d", -1);
    }
}

// sleep for 1 to n sec
void do_sleep(int n) {
    sleep((rand() % n) + 1);
}

// produce and return an item
item produce_item() {
    item it;
    it.num = rand() % MAXNUMBER;
    sprintf(it.str, "%3d", it.num);
    return it;
}

// put item it at pos in buffer
void set_item(int pos, item it) {
    buffer[pos].num = it.num;
    strcpy(buffer[pos].str, it.str);
}

// return item at pos from buffer
item get_item(int pos) {
    item it;
    it.num = buffer[pos].num;
    strcpy(it.str, buffer[pos].str);
    return it;
}

// producer
void *producer(void* rank) {
    for (int i = 0; i < 15; i++) {
        
        // produce next item
        item it = produce_item();

        sem_wait(&empty);
        pthread_mutex_lock(&mutex);

        printf("produced --> item %3d, %s\n", it.num, it.str);
        
        // save item
        set_item(in, it);
        in = (in + 1) % BUFFER_SIZE;

        pthread_mutex_unlock(&mutex);
        sem_post(&full);
    }
}

// consumer
void *consumer(void* rank) {
    for (int i = 0; i < 10; i++) {

        sem_wait(&full);
        pthread_mutex_lock(&mutex);

        // get item
        item it = get_item(out);
        out = (out + 1) % BUFFER_SIZE;

        pthread_mutex_unlock(&mutex);
        sem_post(&empty);

        // consume item
        printf("-------- consumed -------> item %3d, %s\n", it.num, it.str);
    }
}

//--------------------------------
int main(int argc, char* argv[]) {
    pthread_t t_producer, t_consumer;
    srand(time(0));

    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);

    init_buffer();
    
    // Create producer, consumer threads
    pthread_create(&t_producer, NULL, producer, NULL);
    pthread_create(&t_consumer, NULL, consumer, NULL);
    
    // wait till threads are completed
    pthread_join(t_producer, NULL);
    pthread_join(t_consumer, NULL);

    sem_destroy(&full);
    sem_destroy(&empty);

    printf("done\n");
    return 0;
}