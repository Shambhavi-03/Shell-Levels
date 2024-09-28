/*
lvl 5:
The shell supports running executable files as commands within child processes. If the first piece of the input looks like a path (starts with . or /) a child process is created (see man 2 fork) and the command specified by the first argument is executed within the child using the provided arguments (see man 3 execv) similar to the exec builtin. If executing the command fails, the child process prints an error message including a description of the errno (don’t forget to exit the child process). The shell waits for the child to finish running before printing the next prompt (see man 2 waitpid).
*/
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/wait.h>

int main() {
    char cwd[1024];
    char input[1024];
    
    // Loop until EOF (Ctrl+D) is received
    while (1) {
        // Get the current working directory
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            // Print the current directory in the prompt
            printf("%s$ ", cwd);
        } else {
            // Error handling if getcwd fails
            perror("getcwd() error");
            return 1;
        }
        
        // Read a line of user input
        if (fgets(input, sizeof(input), stdin) == NULL) {
            // If EOF is received, break the loop
            break;
        }
        
        // Tokenize the input by whitespace characters
        char *token = strtok(input, " \t\n\r");
        
        // Check if token is not NULL (i.e., if input contains any text)
        if (token != NULL) {
            // Check if the command is "exit"
            if (strcmp(token, "exit") == 0) {
                // Check if there are any arguments provided
                token = strtok(NULL, " \t\n\r");
                if (token != NULL) {
                    printf("Error: Too many arguments for 'exit'\n");
                } else {
                    // Exit the shell with code 0
                    return 0;
                }
            }
            // Check if the command is "cd"
            else if (strcmp(token, "cd") == 0) {
                // Get the next token (the directory path)
                token = strtok(NULL, " \t\n\r");
                if (token == NULL) {
                    printf("Usage: cd <directory>\n");
                } else {
                    // Change the working directory
                    if (chdir(token) != 0) {
                        // Print error message with errno description
                        perror("chdir() error");
                    }
                }
            }
            // Check if the command is "exec"
            else if (strcmp(token, "exec") == 0) {
                // Get the next token (the command to execute)
                token = strtok(NULL, " \t\n\r");
                if (token == NULL) {
                    printf("Usage: exec <command> [arguments...]\n");
                }
                else {
                    // Create an array of arguments for execv
                    char *args[1024];
                    int i = 0;
                    while (token != NULL) {
                        args[i++] = token;
                        token = strtok(NULL, " \t\n\r");
                    }
                    args[i] = NULL;
                    
                    // Create a child process
                    pid_t pid = fork();
                    if (pid == -1) {
                        // Print error message with errno description
                        perror("fork() error");
                    }
                    else if (pid == 0) {
                        // Child process
                        if (execvp(args[0], args) == -1) {
                            // Print error message with errno description
                            perror("execvp() error");
                            exit(errno);
                        }
                    }
                    else {
                        // Parent process
                        int status;
                        if (waitpid(pid, &status, 0) == -1) {
                            // Print error message with errno description
                            perror("waitpid() error");
                        }
                    }
                }
            }
            // Handle unrecognized commands
            else {
                printf("Unrecognized command: %s\n", token);
            }
        }
    }
    
    // Exit with code 0
    return 0;
}