#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N 5 // Number of philosophers and forks

sem_t forks[N];
pthread_t philosophers[N];
int ids[N];

void* philosopher(void* num) {
    int id = *(int*)num;

    // Thinking phase
    printf("Philosopher %d is thinking...\n", id);
    sleep(1);

    // Deadlock Prevention: Asymmetric pick-up order
    if (id % 2 == 0) {
        sem_wait(&forks[id]);                 // Pick up left fork
        sem_wait(&forks[(id + 1) % N]);       // Pick up right fork
    } else {
        sem_wait(&forks[(id + 1) % N]);       // Pick up right fork first
        sem_wait(&forks[id]);                 // Pick up left fork
    }

    // Eating phase
    printf("Philosopher %d is EATING 🍝\n", id);
    sleep(2); // Simulate eating time

    // Put down forks
    sem_post(&forks[id]);
    sem_post(&forks[(id + 1) % N]);

    printf("Philosopher %d finished eating and left the table.\n", id);
    return NULL;
}

int main() {
    // 1. Initialize a semaphore for each fork (1 fork = 1 resource)
    for (int i = 0; i < N; i++) {
        sem_init(&forks[i], 0, 1);
        ids[i] = i;
    }

    // 2. Create philosopher threads
    for (int i = 0; i < N; i++) {
        pthread_create(&philosophers[i], NULL, philosopher, &ids[i]);
    }

    // 3. Wait for all philosophers to finish eating
    for (int i = 0; i < N; i++) {
        pthread_join(philosophers[i], NULL);
    }

    // 4. Clean up semaphores
    for (int i = 0; i < N; i++) {
        sem_destroy(&forks[i]);
    }

    return 0;
}
