#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>
#include<stdlib.h>

int main(){

    pid_t pid = fork();

    if(pid<0){
        perror("Fork failed");
        return 1;
    }
    else if(pid == 0){
        printf("Child Process (PID: %d) is running...\n", getpid());
        execlp("ls", "ls","-l", NULL);
        perror("Exec Failed");
        exit(1);
    }
    else{
        printf("Parent Process (PID: %d) is waiting for child (PID : %d)...\n",getpid(), pid);
        wait(NULL);
        printf("Parent Process: Child has finished executing. Exiting.\n");

    }
    return 0;
}