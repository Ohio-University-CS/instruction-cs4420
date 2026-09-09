#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

// Data that both processes can access
struct SharedData {
    int value;
    int done;
};

int main() {

    // Disable output buffering so changes appear immediately
    setbuf(stdout, NULL);

    // ------------------------------------------------
    // 1. Create shared memory
    // ------------------------------------------------
    int segment_id = shmget(
        IPC_PRIVATE,
        sizeof(struct SharedData),
        IPC_CREAT | 0666
    );

    if (segment_id == -1) {
        perror("shmget");
        exit(1);
    }

    // ------------------------------------------------
    // 2. Attach shared memory
    // ------------------------------------------------
    struct SharedData *shared_memory =
        (struct SharedData *) shmat(segment_id, NULL, 0);

    if (shared_memory == (void *) -1) {
        perror("shmat");
        exit(1);
    }

    // Initialize shared memory
    shared_memory->value = 0;
    shared_memory->done = 0;

    printf("\n===== SHARED MEMORY IPC =====\n");
    printf("Parent and child access the SAME memory.\n\n");

    // ------------------------------------------------
    // 3. Create child process
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

        for (int i = 1; i <= 10; i++) {

            // Directly change shared memory
            shared_memory->value = i;

            printf("CHILD  : changed shared memory --> %d\n", i);

            sleep(1);
        }

        shared_memory->done = 1;

        // ------------------------------------------------
        // 4. Detach shared memory
        // ------------------------------------------------
        shmdt(shared_memory);

        exit(0);
    }

    // =================================================
    // PARENT PROCESS
    // =================================================
    else {

        int previous = 0;

        while (!shared_memory->done) {

            // Parent directly reads the same memory
            if (shared_memory->value != previous) {

                previous = shared_memory->value;

                printf("PARENT : sees shared memory    <-- %d\n\n",
                       shared_memory->value);
            }

            // Small delay so we don't consume 100% CPU
            usleep(100000);
        }

        wait(NULL);

        // ------------------------------------------------
        // 4. Detach shared memory
        // ------------------------------------------------
        shmdt(shared_memory);

        // ------------------------------------------------
        // 5. Remove shared memory
        // ------------------------------------------------
        shmctl(segment_id, IPC_RMID, NULL);

        printf("Shared memory removed.\n");
    }

    return 0;
}