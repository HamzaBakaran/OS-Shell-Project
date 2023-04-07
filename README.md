# SHELL PROJECT GROUP 5
## Group members:
Hamza Bakaran  
Eman Hrustemović  

## Submision files:
[shell5P.c](https://github.com/HamzaBakaran/OS-Shell-Project/blob/master/shell5P.c)  
[example.c](https://github.com/HamzaBakaran/OS-Shell-Project/blob/master/example.c)

## Answers to the questions in the assignment:
Q1:What does the mmap() function do?  
  
The mmap() function is used for mapping between a process address space and either files or devices. When a file is mapped to a process address space, the file can be accessed like an array in the program. This is one of the most efficient ways to access data in the file and provides a seamless coding interface that is natural for a data structure that can be assessed without he abstraction of reading and writing from files. In this article, we are going to discuss how to use the mmap() function in Linux.
Syntax:
```bash
void * mmap (void *address, size_t length, int protect, int flags, int filedes,off_t offset)
```
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
  
Pictorially, we can represent the map function as follows:  
  

![alt text](https://linuxhint.com/wp-content/uploads/2020/07/1-22.jpg)

For unmap the mapped region munmap() function is used :

Syntax:
```bash

int munmap(void *address, size_t length);
```
Return values:
On success, the munmap() returns 0; for failure, the function returns -1.  

Example1.c we allocate memory using mmap. Here we used PROT_READ | PROT_WRITE protection for reading and writing to the mapped region. We used the MAP_PRIVATE | MAP_ANONYMOUS flag. MAP_PRIVATE is used because the mapping region is not shared with other processes, and MAP_ANONYMOUS is used because here, we have not mapped any file. For the same reason, the file descriptor and the offset value is set to 0.  

Example1.c:  
```bash
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
```
In Example2.c we have mapped the file “file1.txt”. First, we have created the file, then mapped the file with the process. We open the file in O_RDONLY mode because here, we only want to read the file.  
  
Example2.c:  
```bash
#include <stdio.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]){

    if(argc < 2){
        printf("File path not mentioned\n");
        exit(0);
    }
   
    const char *filepath = argv[1];
    int fd = open(filepath, O_RDONLY);
    if(fd < 0){
        printf("\n\"%s \" could not open\n",
               filepath);
        exit(1);
    }

    struct stat statbuf;
    int err = fstat(fd, &statbuf);
    if(err < 0){
        printf("\n\"%s \" could not open\n",
                       filepath);
        exit(2);
    }

    char *ptr = mmap(NULL,statbuf.st_size,
            PROT_READ|PROT_WRITE,MAP_SHARED,
            fd,0);
    if(ptr == MAP_FAILED){
        printf("Mapping Failed\n");
        return 1;
    }
    close(fd);

    ssize_t n = write(1,ptr,statbuf.st_size);
    if(n != statbuf.st_size){
        printf("Write failed");
    }

   

    err = munmap(ptr, statbuf.st_size);

    if(err != 0){
        printf("UnMapping Failed\n");
        return 1;
    }
    return 0;
}
```
In Example3.c we have read and then write to the file.  
  

Example3.c
```bash
#include <stdio.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]){

    if(argc < 2){
        printf("File path not mentioned\n");
        exit(0);
    }
   
    const char *filepath = argv[1];
    int fd = open(filepath, O_RDWR);
    if(fd < 0){
        printf("\n\"%s \" could not open\n",
               filepath);
        exit(1);
    }

    struct stat statbuf;
    int err = fstat(fd, &statbuf);
    if(err < 0){
        printf("\n\"%s \" could not open\n",
                       filepath);
        exit(2);
    }

    char *ptr = mmap(NULL,statbuf.st_size,
            PROT_READ|PROT_WRITE,
                        MAP_SHARED,
            fd,0);
    if(ptr == MAP_FAILED){
        printf("Mapping Failed\n");
        return 1;
    }
    close(fd);

    ssize_t n = write(1,ptr,statbuf.st_size);
    if(n != statbuf.st_size){
        printf("Write failed\n");
    }


    // Reverse the file contents
    for(size_t i=0; i \n");
    n = write(1,ptr,statbuf.st_size);
    if(n != statbuf.st_size){
        printf("Write failed\n");
    }

    err = munmap(ptr, statbuf.st_size);

    if(err != 0){
        printf("UnMapping Failed\n");
        return 1;
    }
    return 0;
}
```
In Example4.c first the array is initialized with some values, then the child process updates the values. The parent process reads the values updated by the child because the mapped memory is shared by both processes.  

Example4.c  
```bash
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>

int main(){

    int N=5; // Number of elements for the array
   
    int *ptr = mmap(NULL,N*sizeof(int),
     PROT_READ | PROT_WRITE,
     MAP_SHARED | MAP_ANONYMOUS,
     0,0);    

    if(ptr == MAP_FAILED){
     printf("Mapping Failed\n");
     return 1;
    }

    for(int i=0; i < N; i++){
     ptr[i] = i + 1;
    }

    printf("Initial values of the array elements :\n");
    for (int i = 0; i < N; i++ ){
     printf(" %d", ptr[i] );
    }
    printf("\n");

    pid_t child_pid = fork();
   
    if ( child_pid == 0 ){
     //child
     for (int i = 0; i < N; i++){
         ptr[i] = ptr[i] * 10;
     }
    }
    else{
     //parent
     waitpid ( child_pid, NULL, 0);
     printf("\nParent:\n");

     printf("Updated values of the array elements :\n");
     for (int i = 0; i < N; i++ ){
         printf(" %d", ptr[i] );
     }
     printf("\n");
    }

    int err = munmap(ptr, N*sizeof(int));

    if(err != 0){
     printf("UnMapping Failed\n");
     return 1;
    }
    return 0;
}
```


Conclusion:
The mmap() is a powerful system call. This function should not be used when there are portability issues because this function is only supported by the Linux environment.  
  
Q2:What happens during a context switch? Do we want the OS to perform many or few context
switches?   
  
During a context switch, the operating system (OS) saves the context of the currently running process, including its program counter (PC), general purpose registers, stack pointer, and otherstate information to memory. The OS then loads the context of a different process and starts executing it. This process of saving andrestoring context allows the OS to switch between running processes on the same processor core. We generally do not wantthe OS to perform too many context switches because context switching is an expensive operation in terms of time and resources. Each context switch requires the OS to save and restore the state of a process, which takes time and consumes memory bandwidth.Additionally, each context switch causes the CPU's caches to be flushed, which can degrade performance. In environments withmany processes and high contention for CPU resources, the OS may perform preemptive multitasking, where processes are preempted and their state saved to memory even if they are not blocked on I/O or waiting for a semaphore. In such cases, the OS may perform more context switches.


# How to run our shell:

Firstly you need to compile example.c code with command:
```bash
gcc -o example example.c
```
then
```bash
gcc example.c
```
Then compile shell5P.c code:
```bash
gcc -o shell5P shell5P.c
```
then:
```bash
gcc shell5P.c
```
And type:
```bash
./a.out
```

What we did:

We created a basic implementation of a shell program. It creates a command line interface that allows users to interact with the computer's operating system. The program has several functions that are responsible for reading user input, splitting it into tokens, executing commands, and handling command line arguments.

The main function is responsible for running the main loop of the program. It repeatedly prints a prompt that includes the hostname and username, reads the user's input using **_kash_read_line()_**, splits the input into tokens using **_kash_split_line()_**, checks if the first token is **_"exit"_** and if so, exits the program. Otherwise, it calls What happens during a context switch? Do we want the OS to perform many or few context
switches? **_kash_exec()_** to execute the command.

The **_kash_read_line()_** function reads a line of input from the user using **_getline()_** and returns it as a string.

The **_kash_split_line()_** function splits a string into tokens using **_strtok()_** and returns an array of strings. It dynamically allocates memory for the array and reallocates it as necessary to accommodate more tokens.

The **_kash_exec()_** function is a shell command executor that takes a list of command line arguments as input. The function forks a child process and executes the command in the child process. We implemented **_"rm"_**, **_"cat"_**,**_ "clear"_**, and **_"cowsay"_** as it was asked form us for this project. Additionaly we implemented **_"exit"_** command to exit form our shell and flag **_"-i"_** to "rm" command.So if the command is **_"rm"_** and the second argument is **_"-i"_**, the function prompts the user to confirm the deletion of the file before deleting it using the **_removeFile()_** function. If the second argument is not **_"-i"_**, it directly calls **_removeFile()_** to delete the file. The **_"cat"_** command opens and reads one or more files specified as arguments and prints their contents to the console. The **_"clear"_** command uses the **_system()_** function to execute the **_"clear"_** command, which clears the terminal screen. The **_"cowsay"_** command prints text entered after the command followed by simulating cow with multiple printf lines.By running 
```bash
/example.c 
``` 
in our shell you run code in which is demonstrated use of the **_fork()_**, **_execv()_**, **_kill()_**, and **_wait()_** system calls.

The(example.c) code first calls **_fork()_** to create a new process. If **_fork()_** returns -1, indicating an error, the program terminates with an error message. If it returns 0, it means the child process was created, and if it returns a positive value, it indicates that the parent process has been returned a child process ID.

In the child process, a message is printed to the console indicating that it is the child process and displaying its PID using **_getpid()_**. The child process then sleeps for 5 seconds using the **_sleep()_** function. The **_kill()_** function is then used to send a **SIGTERM** signal to the child process, terminating it. Then the **_execv()_** function is used to execute the ls command with the arguments -la. If **_execv()_** returns, it means that an error occurred, and the **_perror()_** function is used to print an error message to the console. The child process then exits with a status of 1.

In the parent process, a message is printed to indicate its identity, then waits for the child process to terminate using the **_wait()_** system call. When the child process terminates, **_wait()_** returns and sets the status variable to the exit status of the child process. The parent process then prints a message to indicate the child process has exited, along with the child process ID and exit status.


We implemented **_"forkbomb"_** command also additionaly. You can try it by just typing command :
```bash
forkbomb
``` 
The user will be asked if he wants to execute this command, if yes then infinite **_fork_** loop will be executed otherwise user will be asked to enter a new command. 

# Sources:
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


