/*
lvl 0:
The shell prints a prompt, informing the user about which absolute path they are currently in (see man 3 getcwd) followed by a $ and a space (e.g. /your/current/directory$).
The shell then prints a new line and exits without any user interaction.
*/

#include <stdio.h>
#include <unistd.h>

int main() {
    char cwd[1024];
    
    // Get the current working directory
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        // Print the current directory in the prompt
        printf("%s$ ", cwd);
    } else {
        // Error handling if getcwd fails
        perror("getcwd() error");
        return 1;
    }
    
    // Print a new line and exit
    printf("\n");
    
    return 0;
}
