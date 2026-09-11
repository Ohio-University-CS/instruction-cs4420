// sender.c

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

int main() {

    // Create named pipe with read/write permissions for all users
    mkfifo("/tmp/myfifo", 0666);

    printf("===== SENDER =====\n");

    // Open pipe for writing
    int fd = open("/tmp/myfifo", O_WRONLY);

    for (int i = 1; i <= 10; i++) {

        printf("SEND %d ---------->\n", i);

        write(fd, &i, sizeof(int));

        sleep(1);
    }

    close(fd);

    return 0;
}