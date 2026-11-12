#include<stdio.h>
#include<stdlib.h>
#include<semaphore.h>
#include<pthread.h>
#include<unistd.h>

#define TOTAL_ITEMS 10

int shared_buffer;
sem_t empty;
sem_t full;

void* producer(void* arg){
    for(int i =1; i <= TOTAL_ITEMS; i++){
        sem_wait(&empty);
        shared_buffer = i;
        printf("->Produced item: %d\n", i);
        sem_post(&full);
        sleep(1);
    }
    return NULL;
}

void* consumer(void* arg){
    for(int i = 1; i<=TOTAL_ITEMS;i++){
        sem_wait(&full);
        int item = shared_buffer;
        printf("<-Consume item : %d\n", item);
        sem_post(&empty);
        sleep(2);
    }
    return NULL;
}

int main(){
pthread_t producer_thread, consumer_thread;
sem_init(&empty,0,1);
sem_init(&full, 0,0);
pthread_create(&producer_thread, NULL,producer,NULL);
pthread_create(&consumer_thread, NULL, consumer,NULL);

pthread_join(producer_thread,NULL);
pthread_join(consumer_thread, NULL);

sem_destroy(&empty);
sem_destroy(&full);
    return 0;
}