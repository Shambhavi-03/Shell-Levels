/*
lvl 3:
The shell splits the line of input into pieces delimited by whitespace characters (see man 3 isspace).
Instead of just printing “Unrecognized command” the shell shall include the name of the program in the error message (e.g. if the user types cat shell.c, the shell prints “Unrecognized command: cat”).
*/

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

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
            // Print error message with the first token (usually the command)
            printf("Unrecognized command: %s\n", token);
        }
    }
    
    // Exit with code 0
    return 0;
}
