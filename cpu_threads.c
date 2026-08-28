#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

// This function is what every thread will execute.
void *cpu_work(void *arg)
{
    int thread_id = *(int *)arg;

    printf("Thread %d started.\n", thread_id);

    /*
     * Infinite loop:
     *
     * This thread continuously asks the CPU to execute instructions.
     * There is no sleep() here, so the thread is considered CPU-bound.
     *
     * The important OS concept:
     * Even though this thread wants to run forever,
     * the operating system scheduler can interrupt it
     * and give the CPU to another thread/process.
     */
    while (1)
    {
        // Do some meaningless computation so the CPU stays busy.
        volatile unsigned long x = 0;

        for (unsigned long i = 0; i < 1000000; i++)
        {
            x += i;
        }
    }

    return NULL;
}

int main()
{
    /*
     * Number of worker threads we want to create.
     *
     * Try changing this:
     *
     * 1 thread
     * 2 threads
     * 4 threads
     * 8 threads
     * 16 threads
     *
     * Then watch CPU usage in Task Manager / Activity Monitor / top.
     */
    int number_of_threads = 20;

    // Array storing the pthread objects.
    pthread_t threads[number_of_threads];

    // Give each thread an ID so we can identify it.
    int thread_ids[number_of_threads];

    printf("Creating %d CPU-intensive threads...\n", number_of_threads);
    printf("Open Task Manager / Activity Monitor and watch CPU usage.\n\n");

    /*
     * Create the worker threads.
     */
    for (int i = 0; i < number_of_threads; i++)
    {
        thread_ids[i] = i;

        /*
         * pthread_create()
         *
         * Argument 1:
         *     Where pthread should store information about the thread.
         *
         * Argument 2:
         *     Thread configuration.
         *     NULL means use the default settings.
         *
         * Argument 3:
         *     Function the new thread should execute.
         *
         * Argument 4:
         *     Data passed to that function.
         */
        int result = pthread_create(
            &threads[i],
            NULL,
            cpu_work,
            &thread_ids[i]
        );

        if (result != 0)
        {
            printf("Failed to create thread %d\n", i);
            return 1;
        }
    }

    /*
     * Wait for all threads.
     *
     * In this program the threads never finish because they contain
     * while(1), so main() will also stay here forever.
     *
     * You can stop the program using:
     *
     * Ctrl + C
     */
    for (int i = 0; i < number_of_threads; i++)
    {
        pthread_join(threads[i], NULL);
    }

    return 0;
}