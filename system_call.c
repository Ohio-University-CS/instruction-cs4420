// #include <unistd.h>
// #include <string.h>

// int main()
// {
//     const char *msg = "Hello from write()\n";

//     write(STDOUT_FILENO, msg, strlen(msg));

//     return 0;
// }

// #define _GNU_SOURCE

// #include <unistd.h>
// #include <sys/syscall.h>
// #include <string.h>
// #include <stdio.h>

// int main()
// {
//     const char *msg = "Hello through SYS_write\n";

//     printf("USER MODE: before system call\n");

//     long result =
//         syscall(
//             SYS_write,
//             STDOUT_FILENO,
//             msg,
//             strlen(msg)
//         );

//     printf("USER MODE: system call returned %ld\n", result);

//     return 0;
// }


#include <unistd.h>
#include <sys/syscall.h>
#include <stddef.h>
#include <stdio.h>

__attribute__((noinline))
long raw_write(int fd, const char *buffer, size_t count)
{
#if defined(__APPLE__) && defined(__aarch64__)

    printf("[USER MODE] Inside raw_write()\n");
    printf("[USER MODE] Preparing registers for system call...\n");

    register long x0 __asm__("x0") = fd;
    register const char *x1 __asm__("x1") = buffer;
    register size_t x2 __asm__("x2") = count;
    register long x16 __asm__("x16") = SYS_write;

    printf("[USER MODE] About to execute SVC instruction\n");
    printf("------------ USER -> KERNEL ------------\n");

    /*
        EVERYTHING ABOVE THIS POINT
        IS STILL USER MODE.

        svc #0x80 causes the CPU to trap
        into the macOS XNU kernel.

                    USER MODE
                        |
                        v
                    svc #0x80
                        |
                        v
                   KERNEL MODE
    */

    __asm__ volatile(
        "svc #0x80"
        : "+r"(x0)
        : "r"(x1),
          "r"(x2),
          "r"(x16)
        : "memory", "cc"
    );

    /*
        When execution reaches here again,
        the kernel has finished the system call
        and returned us to USER MODE.
    */

    printf("\n------------ KERNEL -> USER ------------\n");
    printf("[USER MODE] Returned from the kernel\n");

    return x0;

#else
#error "This example requires Apple Silicon macOS"
#endif
}

int main()
{
    char message[] =
        "[KERNEL WORK RESULT] Hello from write system call!\n";

    printf("[USER MODE] Program started\n");

    long result = raw_write(
        STDOUT_FILENO,
        message,
        sizeof(message) - 1
    );

    printf("[USER MODE] write() returned %ld bytes\n", result);
    printf("[USER MODE] Program continues normally\n");

    return 0;
}