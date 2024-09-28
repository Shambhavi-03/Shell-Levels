/*
lvl 6:
In the case that the user types something that isn’t a path or a builtin, before printing the unrecognized command error, the shell checks whether a file with that name exists in each of the directories listed in the PATH environment variable in order (see man 3 getenv and man 2 stat). If a file, with that name is found, the search can stop and that file is executed with arguments in a child process. Only if no file is found in any of the directories, should the unrecognized command error be printed (NOTE: you must do the path searching manually, and cannot rely on a member of the exec family that does path searching automatically e.g. execvp).
*/

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/stat.h>

#define MAX_PATH_LENGTH 1024
#define MAX_TOKEN_LENGTH 1024

char *get_path_env();
char *get_path_component(char *path, int index);
int is_executable(char *path);

int main() {
    char cwd[MAX_PATH_LENGTH];
    char input[MAX_PATH_LENGTH];
    char *path_env = get_path_env();
    char *path_component;
    char *path;
    char *token;
    char *args[1024];
    pid_t pid;
    int i = 0;

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
        token = strtok(input, " \t\n\r");

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
                    while (token != NULL) {
                        args[i++] = token;
                        token = strtok(NULL, " \t\n\r");
                    }
                    args[i] = NULL;

                    // Create a child process
                    pid = fork();
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
            else {
                // Check if the command is an executable file in PATH
                path_component = strtok(path_env, ":");
                while (path_component != NULL) {
                    path = malloc(strlen(path_component) + strlen(token) + 2);
                    if (path == NULL) {
                        perror("malloc() error");
                        return 1;
                    }
                    sprintf(path, "%s/%s", path_component, token);
                    if (is_executable(path)) {
                        pid = fork();
                        if (pid == -1) {
                            perror("fork() error");
                        } else if (pid == 0) {
                            if (execv(path, args) == -1) {
                                perror("execv() error");
                                exit(errno);
                            }
                        } else {
                            int status;
                            if (waitpid(pid, &status, 0) == -1) {
                                perror("waitpid() error");
                            }
                        }
                        break;
                    }
                    free(path);
                    path_component = strtok(NULL, ":");
                }
                if (path_component == NULL) {
                    printf("Error: Command not found\n");
                }
            }
        }
    }

    return 0;
}



                   