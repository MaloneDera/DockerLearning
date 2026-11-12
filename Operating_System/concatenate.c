#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    int fd1, fd2, fd3;
    char buffer[1024];
    ssize_t bytes_read;

    // 1. Open t1.txt for reading
    fd1 = open("t1.txt", O_RDONLY);
    if (fd1 == -1) {
        perror("Failed to open t1.txt");
        exit(1);
    }

    // 2. Open t2.txt for reading
    fd2 = open("t2.txt", O_RDONLY);
    if (fd2 == -1) {
        perror("Failed to open t2.txt");
        close(fd1);
        exit(1);
    }

    // 3. Open t3.txt for writing (Create if missing, truncate if it exists)
    fd3 = open("t3.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd3 == -1) {
        perror("Failed to open/create t3.txt");
        close(fd1);
        close(fd2);
        exit(1);
    }

    // 4. Read from t1.txt and write into t3.txt chunk by chunk
    while ((bytes_read = read(fd1, buffer, sizeof(buffer))) > 0) {
        write(fd3, buffer, bytes_read);
    }

    // 5. Read from t2.txt and append into t3.txt chunk by chunk
    while ((bytes_read = read(fd2, buffer, sizeof(buffer))) > 0) {
        write(fd3, buffer, bytes_read);
    }

    // 6. Clean up: Close all file descriptors
    close(fd1);
    close(fd2);
    close(fd3);

    printf("Successfully concatenated t1.txt and t2.txt into t3.txt!\n");

    return 0;
}
