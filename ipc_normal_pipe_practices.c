#include <stdio.h>
#include <unistd.h>

int main() {
    int fd[2];
    pipe(fd);

    write(fd[1], "ABC", 3);
    write(fd[1], "DEF", 3);

    char buf[5] = {0};
    // read(fd[0], buf, 4);
    // int a;
    printf("%s\n", buf);

    char buf1[5] = {0};
    read(fd[0], buf1, 4);

    printf("%s\n", buf1);

    close(fd[0]);
    close(fd[1]);

    return 0;
}