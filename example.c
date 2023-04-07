#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {

/*
The fork() system call is used to create a new process, which is a copy of the calling process.
 The fork() call returns different values to the parent and child processes. 
 If fork() returns -1, an error occurred and the program terminates. 
 If fork() returns 0, it is the child process. 
 Otherwise, it is the parent process, and fork() returns the process ID (PID) of the child process.
*/
    int status;
    pid_t child_pid = fork();

    if (child_pid == -1) {
        perror("fork error");
        exit(1);
    }
   else if (child_pid == 0) { // child process
   /*
   In the child process, a message is printed to the console indicating that it is the child process and displaying its PID using getpid(). 
   The child process then sleeps for 5 seconds using the sleep() function.
   */
    printf("I am the child process with PID %d\n", getpid());
    sleep(5);
    printf("Child process with PID %d is awake\n", getpid());
    /*
    The kill() function is then used to send a SIGTERM signal to the child process, terminating it.
    */
    kill(getpid(), SIGTERM);
    printf("Child process with PID %d has been killed\n", getpid());
    /*
    The execv() function is then used to execute the ls command with the arguments -la. 
    The first argument specifies the path to the executable file to be run, and the second argument is an array of arguments passed to the executable.
    */
    char *args[] = {"ls", "-la", NULL};
    execv("/bin/ls", args);
    /*
    If execv() returns, it means that an error occurred, and the perror() function is used to print an error message to the console. The child process then exits with a status of 1.
    */
    perror("exec error");
    exit(1);
}

    else { // parent process
    /*
    If the program is running in the parent process, it prints a message to indicate its identity, then waits for the child process to terminate using the wait() system call.
    When the child process terminates, wait() returns and sets the status variable to the exit status of the child process. 
    The parent process then prints a message to indicate the child process has exited, along with the child process ID and exit status.
    */
        printf("I am the parent process with PID %d\n", getpid());
        wait(&status);
        printf("Child process with PID %d exited with status %d\n", child_pid, WEXITSTATUS(status));
    }

    return 0;
}
