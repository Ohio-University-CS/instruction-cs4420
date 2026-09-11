// receiver.c

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main() {

    printf("===== RECEIVER =====\n");

    // Open same named pipe for reading
    int fd = open("/tmp/myfifo", O_RDONLY);

    int value;

    while (read(fd, &value, sizeof(int)) > 0) {

        printf("     <---------- RECEIVE %d\n", value);
    }

    close(fd);

    return 0;
}