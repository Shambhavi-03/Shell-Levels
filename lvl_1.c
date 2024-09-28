/*
lvl 1:
The shell reads lines of user input, but doesn’t need to do anything with them. It just prints a new prompt before each line.
The shell loops until it gets EOF from user input (ctrl+d).
The shell exits with code 0.
*/

#include <stdio.h>
#include <unistd.h>

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
    }
    
    // Exit with code 0
    return 0;
}
