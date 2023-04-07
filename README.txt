Group members:
-Hamza Bakaran
-Eman Hrustemović

Submision files:
shell5P.c
example.c

Answers to the questions in the assignment:
Q1:
The mmap() function is used for mapping between a process address space and either files or devices. When a file is mapped to a process address space, the file can be accessed like an array in the program. This is one of the most efficient ways to access data in the file and provides a seamless coding interface that is natural for a data structure that can be assessed without he abstraction of reading and writing from files. In this article, we are going to discuss how to use the mmap() function in Linux.
Syntax:

void * mmap (void *address, size_t length, int protect, int flags, int filedes,
off_t offset)
Arguments:
The function takes 6 arguments:


1. address:
This argument gives a preferred starting address for the mapping. If another mapping does not exist there, then the kernel will pick a nearby page boundary and create the mapping; otherwise, the kernel picks a new address. If this argument is NULL, then the kernel can place the mapping anywhere it sees fit.

2. length:
This is the number of bytes which to be mapped.

3. protect:
This argument is used to control what kind of access is permitted. This argument may be logical ‘OR’ of the following flags PROT_READ | PROT_WRITE | PROT_EXEC | PROT_NONE.  The access types of read, write and execute are the permissions on the content.


4. flags:
This argument is used to control the nature of the map. Following are some common values of the flags:

MAP_SHARED: This flag is used to share the mapping with all other processes, which are mapped to this object. Changes made to the mapping region will be written back to the file.
MAP_PRIVATE: When this flag is used, the mapping will not be seen by any other processes, and the changes made will not be written to the file.
MAP_ANONYMOUS / MAP_ANON: This flag is used to create an anonymous mapping. Anonymous mapping means the mapping is not connected to any files. This mapping is used as the basic primitive to extend the heap.
MAP_FIXED: When this flag is used, the system has to be forced to use the exact mapping address specified in the address If this is not possible, then the mapping will be failed.
5. filedes:
This is the file descriptor which has to be mapped.
6. offset:
This is offset from where the file mapping started. In simple terms, the mapping connects to (offset) to (offset+length-1) bytes for the file open on filedes descriptor.

Return values:
On success, the mmap() returns 0; for failure, the function returns MAP_FAILED.

For unmap the mapped region munmap() function is used :

Syntax:

int munmap(void *address, size_t length);

Return values:
On success, the munmap() returns 0; for failure, the function returns -1.

Examples:
#include <stdio.h>
#include <sys/mman.h>

int main(){

int N=5;
int *ptr = mmap ( NULL, N*sizeof(int),
 PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0 );

if(ptr == MAP_FAILED){
    printf("Mapping Failed\n");
    return 1;
}

for(int i=0; i<N; i++)
    ptr[i] = i*10;

for(int i=0; i<N; i++)
    printf("[%d] ",ptr[i]);

printf("\n");
int err = munmap(ptr, 10*sizeof(int));
if(err != 0){
    printf("UnMapping Failed\n");
    return 1;
}

return 0;
}

Q2:
During a context switch, the operating system (OS) saves the
context of the currently running process, including its program
counter (PC), general purpose registers, stack pointer, and other
state information to memory. The OS then loads the context of a
different process and starts executing it. This process of saving and
restoring context allows the OS to switch between running
processes on the same processor core. We generally do not want
the OS to perform too many context switches because context
switching is an expensive operation in terms of time and resources.
Each context switch requires the OS to save and restore the state of
a process, which takes time and consumes memory bandwidth.
Additionally, each context switch causes the CPU's caches to be
flushed, which can degrade performance. In environments with
many processes and high contention for CPU resources, the OS
may perform preemptive multitasking, where processes are
preempted and their state saved to memory even if they are not
blocked on I/O or waiting for a semaphore. In such cases, the OS may
perform more context switches.


How to run our shell:

Firstly you need to compile example.c code with command:
gcc-o example example.c
gcc example.c
Then compile shell5P.c code:
gcc-o shell5P shell5P.c
gcc shell5P.c
And type:
./a.out

What we did:

We created a basic implementation of a shell program. It creates a command line interface that allows users to interact with the computer's operating system. The program has several functions that are responsible for reading user input, splitting it into tokens, executing commands, and handling command line arguments.

The main function is responsible for running the main loop of the program. It repeatedly prints a prompt that includes the hostname and username, reads the user's input using kash_read_line(), splits the input into tokens using kash_split_line(), checks if the first token is "exit" and if so, exits the program. Otherwise, it calls kash_exec() to execute the command.

The kash_read_line() function reads a line of input from the user using getline() and returns it as a string.

The kash_split_line() function splits a string into tokens using strtok() and returns an array of strings. It dynamically allocates memory for the array and reallocates it as necessary to accommodate more tokens.

The kash_exec() function is a shell command executor that takes a list of command line arguments as input. The function forks a child process and executes the command in the child process. We implemented "rm", "cat", "clear", and "cowsay" as it was asked form us for this project. Additionaly we implemented "exit" comand and flag "-i" to "rm" command.So if the command is "rm" and the second argument is "-i", the function prompts the user to confirm the deletion of the file before deleting it using the removeFile() function. If the second argument is not "-i", it directly calls removeFile() to delete the file. The cat command opens and reads one or more files specified as arguments and prints their contents to the console. The clear command uses the system() function to execute the "clear" command, which clears the terminal screen. The cowsay command prints text entered after the command followed by simulating cow with multiple printf lines.By running ./example.c in our shell you run code in which is demonstrated use of the fork(), execv(), kill(), and wait() system calls.

The(example.c) code first calls fork() to create a new process. If fork() returns -1, indicating an error, the program terminates with an error message. If it returns 0, it means the child process was created, and if it returns a positive value, it indicates that the parent process has been returned a child process ID.

In the child process, a message is printed to the console indicating that it is the child process and displaying its PID using getpid(). The child process then sleeps for 5 seconds using the sleep() function. The kill() function is then used to send a SIGTERM signal to the child process, terminating it. Then the execv() function is used to execute the ls command with the arguments -la. If execv() returns, it means that an error occurred, and the perror() function is used to print an error message to the console. The child process then exits with a status of 1.

In the parent process, a message is printed to indicate its identity, then waits for the child process to terminate using the wait() system call. When the child process terminates, wait() returns and sets the status variable to the exit status of the child process. The parent process then prints a message to indicate the child process has exited, along with the child process ID and exit status.


We implemented "forkbomb" command also additionaly. You can try it by just typing command "forkbomb". The user will be asked if he wants to execute this command, if yes then infinite fork loop will be executed otherwise user will be asked to enter a new command. 

Sources:
https://www.geeksforgeeks.org/making-linux-shell-c/
https://www.youtube.com/watch?v=4jYFqFsu03A
https://blog.ehoneahobed.com/building-a-simple-shell-in-c-part-1
https://www.youtube.com/watch?v=spD11nnLsLE
https://www.geeksforgeeks.org/fork-bomb/
https://stackoverflow.com/questions/5947742/how-to-change-the-output-color-of-echo-in-linux
https://www.tutorialspoint.com/what-is-context-switching-in-operating-system
https://en.m.wikipedia.org/wiki/Context_switch
https://www.quora.com/How-do-I-create-my-own-shell-in-Linux
https://en.wikipedia.org/wiki/Unix_shell
https://linuxhint.com/using_mmap_function_linux/#:~:text=The%20mmap()%20function%20is,an%20array%20in%20the%20program


