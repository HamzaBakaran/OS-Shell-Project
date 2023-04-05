#include <stdbool.h>

#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include <sys/wait.h>

#include <unistd.h>

#include <limits.h>

#include <fcntl.h>

#include <errno.h>

#include <sys/types.h>

char * kash_read_line();
char ** kash_split_line(char * line);
void kash_exec(char ** args);

void main() {
  char hostname[HOST_NAME_MAX + 1];
  char * username = getenv("USER");
  gethostname(hostname, HOST_NAME_MAX + 1);

  /*
  This is the main loop of the program, which runs repeatedly until the user enters the "exit" command. 
  It prints a prompt that includes the hostname and username, reads the user's input using kash_read_line, splits the input into tokens using kash_split_line, checks if the first token is "exit" and if so, exits the program. Otherwise, it calls kash_exec to execute the command.
  */
  while (true) {
    //printf("Hamza");
    /*
    This code retrieves the hostname and the current user's name.
    */
    printf("\033[0;32m %s@\033[0m\033[0;34m%s:~$ \033[0m", hostname, username);
    char * line = kash_read_line();
    char ** tokens = kash_split_line(line);

    if (tokens[0] != NULL) {
      if (strcmp(tokens[0], "exit") == 0) {
        free(tokens);
        free(line);
        exit(0);
      }
      kash_exec(tokens);
    }

    free(tokens);
    free(line);

  }
}
/*
This function reads a line of input from the user using getline and returns it as a string.
*/
char * kash_read_line() {
  char * line = NULL;
  size_t buflen = 0;
  getline( & line, & buflen, stdin);
  return line;
}
/*
This function splits a string into tokens using strtok and returns an array of strings. 
It dynamically allocates memory for the array and reallocates it as necessary to accommodate more tokens.
*/
char ** kash_split_line(char * line) {
  int length = 0;
  int capacity = 16;
  char ** tokens = malloc(capacity * sizeof(char * ));

  char * delimiters = " \t\r\n";
  char * token = strtok(line, delimiters);

  while (token != NULL) {
    tokens[length] = token;
    length++;

    if (length >= capacity) {
      capacity = (int)(capacity * 1.5);
      tokens = realloc(tokens, capacity * sizeof(char * ));
    }

    token = strtok(NULL, delimiters);
  }

  tokens[length] = NULL;
  return tokens;
}
/*
Remove file function for "rm" command
*/
void removeFile(char * file) {
  int status;
  status = remove(file);
  if (status == 0) {
    printf("successfull\n");
  } else {
    printf("Unsuccessfull fir\n");
  }

}
/*
The function kash_exec(char ** args) is a shell command executor that takes a list of command line arguments as input. 
The function forks a child process and executes the command in the child process
*/
void kash_exec(char ** args) {
  pid_t child_pid = fork();

  if (child_pid == 0) {
    //execvp(args[0], args);
    /*
    If the command is "rm" and the second argument is "-i", the function prompts the user to confirm the deletion of the file before deleting it using the removeFile() function. 
    If the second argument is not "-i", it directly calls removeFile() to delete the file
    */
    if (strcmp(args[0], "rm") == 0) {
      if (strcmp(args[1], "-i") == 0) {
        char decision;
        printf("Remove %s ?(y/n)", args[2]);
        scanf(" %c", & decision);

        if (decision == 'y') {
          removeFile(args[2]);

          exit(1);
        } else exit(1);
      }

      removeFile(args[1]);

    }
    /*
    "cat" command implementation.It opens and reads one or more files specified as arguments, and prints their contents to the console. 
    It does this by first opening the file(s) with the fopen() function, then reading the contents of the file(s) character by character with fgetc() function,
     and finally printing each character to the console with putchar().
    */
    else if (strcmp(args[0], "cat") == 0) {
      FILE * fp;
      char ch;

      for (int i = 1; args[i] != NULL; i++) {
        fp = fopen(args[i], "r");
        if (fp == NULL) {
          perror("cat");
          exit(EXIT_FAILURE);
        }

        while ((ch = fgetc(fp)) != EOF) {
          putchar(ch);
        }
        printf("\n");
        fclose(fp);
      }
    }
    /*
    "clear" command implementation.Using the system() function to execute the "clear" command, which clears the terminal screen.
    */
    else if (strcmp(args[0], "clear") == 0) {
      system("clear");
    }
    /*
    "cowsay" command implementation.It prints text entered after command followed by simulating cow with multiple printf lines
    */
    else if (strcmp(args[0], "cowsay") == 0) {
      int i = 1;

      printf("\n");

      printf("<");
      while (args[i] != NULL) {
        printf(" %s ", args[i]);
        i++;
      }
      printf(">");
      printf("\n");
      printf(" -------\n");
      printf("        \\   ^__^ \n");
      printf("         \\  (oo)\\_______\n");
      printf("            (__)\\      )\\ / \\  \n");
      printf("                ||----w |\n");
      printf("                ||     ||\n");

    }
    /*
    execvp() is a function that replaces the current running process with a new process. 
    In this case, it will replace the current process with the process defined in the "./example" executable file, with the arguments passed in the args array.
    */
    else if (strcmp(args[0], "./example") == 0) {
      execvp(args[0], args);
    }
    /*
    Default output if commands are not entered correctly 
    */
    else {
      printf("Command does not exist\n");
    }
    exit(1);
  }
  /*
  This block of code waits for the child process to finish and checks its exit status. 
  If the child process completes successfully, it exits the loop, but if the process is interrupted or signaled, it will continue to wait. 
  If the child process fails to execute, an error message is  printed.
  */
  else if (child_pid > 0) {
    int status;
    do {
      waitpid(child_pid, & status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  } else {
    perror("kash");
  }
}
