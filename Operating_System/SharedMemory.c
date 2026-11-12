#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>

#define SHM_SIZE 1024 // Size of the shared memory segment in bytes

int main() {
    key_t key;
    int shmid;
    char *str;

    // 1. Generate a unique key using ftok
    key = ftok(".", 's');
    if (key == -1) {
        perror("ftok failed");
        exit(1);
    }

    // 2. Create the shared memory segment (0666 = read/write permissions)
    shmid = shmget(key, SHM_SIZE, 0666 | IPC_CREAT);
    if (shmid == -1) {
        perror("shmget failed");
        exit(1);
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        exit(1);
    }

    if (pid == 0) {
        // ==========================================
        // CHILD PROCESS (Reader)
        // ==========================================
        // Attach the shared memory segment to the child's virtual address space
        str = (char*) shmat(shmid, NULL, 0);
        if (str == (char*)(-1)) {
            perror("shmat failed");
            exit(1);
        }

        // Read directly from the shared memory address
        printf("Child read: %s\n", str);

        // Detach the memory segment when done
        shmdt(str);
    } 
    else {
        // ==========================================
        // PARENT PROCESS (Writer)
        // ==========================================
        // Attach the shared memory segment to the parent's virtual address space
        str = (char*) shmat(shmid, NULL, 0);
        if (str == (char*)(-1)) {
            perror("shmat failed");
            exit(1);
        }

        // Write directly into the shared memory pointer
        strcpy(str, "Hello through high-speed shared memory!");
        printf("Parent wrote to shared memory.\n");

        // Detach the memory segment
        shmdt(str);

        // Wait for the child process to finish reading
        wait(NULL);

        // Completely destroy/remove the shared memory segment from the kernel
        shmctl(shmid, IPC_RMID, NULL);
        printf("Shared memory segment destroyed.\n");
    }

    return 0;
}
