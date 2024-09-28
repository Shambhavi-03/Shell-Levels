/*
lvl 4:
The shell supports a few builtin commands (exit, cd and exec). If the first piece of the input is not “exit” or “cd” or “exec” it will still print the unrecognized command message, otherwise: * exit: takes no arguments (prints error if they are provided) and closes the shell (return value of 0) * cd: takes exactly one argument (otherwise it prints usage info) and changes the working directory of the shell process (see man 2 chdir). If chdir does not accept the path provided, an error message including a description of the errno is printed. The new working directory is reflected in the prompt. * exec: takes at least one argument (maybe more) and replaces the shell with an instance of the command specified (see man 3 execv). If execution fails, an error message is printed including a description of the errno, and the shell continues. Otherwise, having been replaced, the shell does not return.
*/
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <errno.h>

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
                    
                    // Replace the shell with the specified command
                    if (execvp(args[0], args) == -1) {
                        // Print error message with errno description
                        perror("execvp() error");
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