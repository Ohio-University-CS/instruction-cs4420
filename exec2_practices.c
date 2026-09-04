#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    printf("we are in exec2_practices.c\n");
    printf("args: %d\n", argc);
    printf("PID of exec2_practices.c = %d\n", getpid());

    return 0;
}