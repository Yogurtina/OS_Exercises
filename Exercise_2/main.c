#include <stdio.h>
#include <pthread.h>
#include <semaphore.h> // posix semaphores
#include <fcntl.h> // for O_* constants

// global variable
sem_t A2; 
sem_t B1;
sem_t C1;
sem_t C2;
sem_t X;
sem_t Y;
sem_t Z;

int a2, b1, c1, c2, x, y, z;

// thread function
void *t1() {

    int a1= 10;
    a2 =11;

    sem_post(&A2);

    sem_wait(&C1);

    y = a1 + c1;
    sem_post(&Y);
    
    sem_wait(&X);

    printf(" %d\n",  x);
    sem_post(&X);

    return NULL;
}

// thread function
void *t2() {
    b1 =20;
    sem_post(&B1);

    int b2 = 21;

    sem_wait(&C2);
    int w = b2+c2;

    sem_wait(&Z);
    sem_wait(&Y);

    x = z - y + w;
    sem_post(&X);
    return NULL;

}

void *t3(){
    c1 = 30;
    sem_post(&C1);
    c2 = 31;
    sem_post(&C2);

    sem_wait(&A2);
    sem_wait(&B1);
    z = a2+b1;

    sem_post(&Z);
    return NULL;
    
}

int main(){

    pthread_t thread1, thread2, thread3;

    sem_init(&A2, 0, 0);
    sem_init(&B1 ,0, 0);
    sem_init(&C1, 0, 0);
    sem_init(&C2, 0, 0);
    sem_init(&X,  0, 0);
    sem_init(&Y,  0, 0);
    sem_init(&Z,  0, 0);

    pthread_create(&thread1, NULL, t1, NULL);
    pthread_create(&thread2, NULL, t2, NULL);
    pthread_create(&thread3, NULL, t3, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);

    return 0;

}

// gcc -g -Wall main.c -lpthread -lrt -o main