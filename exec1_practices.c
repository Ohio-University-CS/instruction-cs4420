#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    printf("PID of exec1_practices.c = %d\n", getppid());
    char *args[] = {"Hello", "Operating", "System", NULL};
    fork();
    printf("first fork\n");
    execvp("./exec2_practices", args);
    printf("Back to exec1_practices.c\n");
    fork();
    printf("second fork\n");
    
    return 0;
}