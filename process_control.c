#include <sys/types.h> /* pid_t */
#include <sys/wait.h>  /* wait */
#include <stdio.h>    /* printf, stderr, fprintf */ 
#include <unistd.h>    /* _exit, fork */ 
int main()
{
	pid_t  pid;
    printf("Init PID Variable: %ld\n",
           (long)pid);
	/* fork another process */
	pid = fork();          
	if (pid < 0) { 		/* error occurred */
		fprintf(stderr, "Fork Failed");
		return 1;
	}
	else if (pid == 0) { 	/* child process */
		/* overlays address space with the UNIX command /bin/ls */
		printf("Child: PID Variable: %ld\n",
           (long)pid);
        printf("Child: My PID: %ld, Parent PID: %ld\n",
           (long)getpid(), (long)getppid());
        execlp("/bin/ls", "ls", NULL);
	}
	else { 			/* parent process if pid > 0 */
		/* parent will wait for the child */
		printf("Parent: PID Variable: %ld\n",
           (long)pid);
        printf("Parent: My PID: %ld, Parent PID: %ld\n",
           (long)getpid(), (long)getppid());
        wait(NULL);
		printf ("Child Complete");
	}
	return 0;
}
