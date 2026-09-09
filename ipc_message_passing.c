#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {

    setbuf(stdout, NULL);

    int pipefd[2];

    // ------------------------------------------------
    // 1. Create communication pipe
    //
    // pipefd[0] = read end
    // pipefd[1] = write end
    // ------------------------------------------------
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(1);
    }

    printf("\n===== MESSAGE PASSING IPC =====\n");
    printf("Child must SEND data to the parent.\n\n");

    // ------------------------------------------------
    // 2. Create child process
    // ------------------------------------------------
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(1);
    }

    // =================================================
    // CHILD PROCESS
    // =================================================
    if (pid == 0) {

        // Child does not need the read side
        close(pipefd[0]);

        for (int i = 1; i <= 10; i++) {

            printf("CHILD  : SEND %d ---------->\n", i);

            // Explicitly SEND the value
            write(pipefd[1], &i, sizeof(int));

            sleep(1);
        }

        close(pipefd[1]);

        exit(0);
    }

    // =================================================
    // PARENT PROCESS
    // =================================================
    else {

        // Parent does not need the write side
        close(pipefd[1]);

        int value;

        // Explicitly RECEIVE values
        while (read(pipefd[0],
                    &value,
                    sizeof(int)) > 0) {

            printf("PARENT :      <---------- RECEIVE %d\n\n",
                   value);
        }

        close(pipefd[0]);

        wait(NULL);

        printf("Pipe closed.\n");
    }

    return 0;
}