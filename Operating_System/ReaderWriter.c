#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t wrt;            // Controls exclusive access for writers
pthread_mutex_t mutex; // Protects the read_count variable
int read_count = 0;   // Keeps track of active readers
int shared_data = 0;  // The resource being read/written

void* reader(void* arg) {
    int id = *(int*)arg;

    // --- READER ENTRY ---
    pthread_mutex_lock(&mutex);
    read_count++;
    if (read_count == 1) {
        sem_wait(&wrt); // First reader locks out writers
    }
    pthread_mutex_unlock(&mutex);

    // --- READING ---
    printf("Reader %d is reading data: %d\n", id, shared_data);
    sleep(1); // Simulate reading time

    // --- READER EXIT ---
    pthread_mutex_lock(&mutex);
    read_count--;
    if (read_count == 0) {
        sem_post(&wrt); // Last reader releases writer lock
    }
    pthread_mutex_unlock(&mutex);

    return NULL;
}

void* writer(void* arg) {
    int id = *(int*)arg;

    // --- WRITER ENTRY ---
    sem_wait(&wrt); // Wait for exclusive access

    // --- WRITING ---
    shared_data++;
    printf("Writer %d updated data to: %d\n", id, shared_data);
    sleep(1); // Simulate writing time

    // --- WRITER EXIT ---
    sem_post(&wrt); // Release exclusive access

    return NULL;
}

int main() {
    pthread_t r[3], w[2];
    int id[3] = {1, 2, 3};

    // Initialize synchronization tools
    sem_init(&wrt, 0, 1);
    pthread_mutex_init(&mutex, NULL);

    // Create reader and writer threads
    pthread_create(&r[0], NULL, reader, &id[0]);
    pthread_create(&r[1], NULL, reader, &id[1]);
    pthread_create(&w[0], NULL, writer, &id[0]);
    pthread_create(&r[2], NULL, reader, &id[2]);
    pthread_create(&w[1], NULL, writer, &id[1]);

    // Wait for all threads to finish
    for (int i = 0; i < 3; i++) pthread_join(r[i], NULL);
    for (int i = 0; i < 2; i++) pthread_join(w[i], NULL);

    // Cleanup
    sem_destroy(&wrt);
    pthread_mutex_destroy(&mutex);

    return 0;
}
