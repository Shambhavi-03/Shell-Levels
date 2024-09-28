/*
lvl 2:
If the user types any text, the shell prints “Unrecognized command” (but does not exit the loop).
However, if the user just hits enter without typing anything, no error message is printed.
*/

#include <stdio.h>
#include <unistd.h>
#include <string.h>

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
        
        // Check if the input is not empty
        if (strlen(input) > 1) {
            // Print error message for unrecognized command
            printf("Unrecognized command\n");
        }
    }
    
    // Exit with code 0
    return 0;
}
