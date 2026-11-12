#include<stdio.h>
#include<sys/wait.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>


int main(){
int pipfd[2];
pid_t pid;
char write_msg[] = "Hello from the parent process";
char read_msg[100];

if(pipe(pipfd)==-1){
    perror("Pipe Failed");
    return 1;
}
pid = fork();

if(pid<0){
perror("fork failed");
return 1;
}
if(pid == 0){
    close(pipfd[1]);
    read(pipfd[0], read_msg, sizeof(read_msg));
    printf("Child recieved  : %s\n ", read_msg);
    close(pipfd[0]);
}
else {
close(pipfd[0]);
write(pipfd[1], write_msg,strlen(write_msg)+1);
printf("Parent sent : %s \n", write_msg);
close(pipfd[1]);
wait(NULL);
}
    return 0;
}