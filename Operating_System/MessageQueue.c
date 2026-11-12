#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <sys/wait.h>

// Define the structure of the message
struct message {
    long msg_type;
    char msg_text[100];
};

int main() {
    key_t key;
    int msgid;
    struct message msg;

    // 1. Generate a unique key using ftok
    key = ftok(".", 'b');
    if (key == -1) {
        perror("ftok failed");
        exit(1);
    }

    // 2. Create a message queue and get its ID
    // 0666 sets read/write permissions, IPC_CREAT creates it if it doesn't exist
    msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget failed");
        exit(1);
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        exit(1);
    }

    if (pid == 0) {
        // ==========================================
        // CHILD PROCESS (Receiver)
        // ==========================================
        // Receive the message from the queue (msg_type = 1)
        msgrcv(msgid, &msg, sizeof(msg.msg_text), 1, 0);
        printf("Child received message: %s\n", msg.msg_text);
    } 
    else {
        // ==========================================
        // PARENT PROCESS (Sender)
        // ==========================================
        msg.msg_type = 1; // Set message type identifier
        strcpy(msg.msg_text, "Hello through the message queue!");

        // Send the message into the queue
        msgsnd(msgid, &msg, sizeof(msg.msg_text), 0);
        printf("Parent sent message: %s\n", msg.msg_text);

        // Wait for the child to finish reading
        wait(NULL);

        // Clean up and delete the message queue from the system
        msgctl(msgid, IPC_RMID, NULL);
        printf("Message queue destroyed.\n");
    }

    return 0;
}