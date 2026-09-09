////////////////////////////////// Practices 1

#include <stdio.h>
#include <unistd.h>

int main()
{
    /* fork a child process */
    fork();

    /* fork another child process */
    fork();

    /* and fork another */
    fork();

    return 0;
}

////////////////////////////////// Practices 2

// #include <sys/types.h>
// #include <stdio.h>
// #include <unistd.h>

// int main()
// {
//     pid_t pid, pid1;

//     /* fork a child process */
//     pid = fork();

//     if (pid < 0) { /* error occurred */
//         fprintf(stderr, "Fork Failed");
//         return 1;
//     }
//     else if (pid == 0) { /* child process */
//         pid1 = getpid();
//         printf("child: pid = %d", pid);   /* A */
//         printf("child: pid1 = %d", pid1); /* B */
//     }
//     else { /* parent process */
//         pid1 = getpid();
//         printf("parent: pid = %d", pid);   /* C */
//         printf("parent: pid1 = %d", pid1); /* D */
//         wait(NULL);
//     }

//     return 0;
// }


////////////////////////////////// Practices 3

#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

int value = 5;

int main()
{
    pid_t pid;

    pid = fork();

    if (pid == 0) { /* child process */
        value += 15;
        return 0;
    }
    else if (pid > 0) { /* parent process */
        wait(NULL);
        printf("PARENT: value = %d", value); /* LINE A */
        return 0;
    }
}