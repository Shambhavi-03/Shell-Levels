

## Levels

### Level 0
- The shell prints a prompt displaying the absolute path the user is currently in (see `man 3 getcwd`), followed by a `$` and a space (e.g., `/your/current/directory$`).
- The shell then prints a new line and exits without any user interaction.

### Level 1
- The shell reads lines of user input but does not perform any actions with them. It simply prints a new prompt before each line.
- The shell loops until it receives EOF from user input (Ctrl + D).
- The shell exits with code 0.

### Level 2
- If the user types any text, the shell prints "Unrecognized command" but does not exit the loop.
- If the user hits enter without typing anything, no error message is printed.

### Level 3
- The shell splits the input line into pieces delimited by whitespace characters (see `man 3 isspace`).
- Instead of printing "Unrecognized command," the shell includes the name of the program in the error message (e.g., if the user types `cat shell.c`, the shell prints "Unrecognized command: cat").

### Level 4
- The shell supports a few built-in commands (`exit`, `cd`, and `exec`):
  - **exit**: Takes no arguments (prints an error if they are provided) and closes the shell (return value of 0).
  - **cd**: Takes exactly one argument (otherwise, it prints usage info) and changes the working directory of the shell process (see `man 2 chdir`). If `chdir` does not accept the path provided, an error message including a description of `errno` is printed. The new working directory is reflected in the prompt.
  - **exec**: Takes at least one argument (maybe more) and replaces the shell with an instance of the specified command (see `man 3 execv`). If execution fails, an error message is printed including a description of `errno`, and the shell continues. Otherwise, having been replaced, the shell does not return.

### Level 5
- The shell supports running executable files as commands within child processes. If the first piece of the input looks like a path (starts with `.` or `/`), a child process is created (see `man 2 fork`), and the command specified by the first argument is executed within the child using the provided arguments (see `man 3 execv`), similar to the `exec` built-in. If executing the command fails, the child process prints an error message including a description of `errno` (don’t forget to exit the child process). The shell waits for the child to finish running before printing the next prompt (see `man 2 waitpid`).

### Level 6
- If the user types something that isn’t a path or a built-in command, the shell checks whether a file with that name exists in each of the directories listed in the `PATH` environment variable in order (see `man 3 getenv` and `man 2 stat`). If a file with that name is found, it is executed with arguments in a child process. Only if no file is found in any of the directories should the "Unrecognized command" error be printed (NOTE: you must do the path searching manually and cannot rely on a member of the `exec` family that does path searching automatically, e.g., `execvp`).

### Level 7
- Before processing the entered commands, the shell performs home directory substitution on the pieces (command name or arguments) that start with `~`.
- The shell determines a username string by taking a substring of the piece after the `~` until the end of the string or the first `/`, whichever comes first.
- If the username string is empty, the `~` is replaced with the value of the `HOME` environment variable (see `man 3 getenv`).
- If the username string is not empty, the shell attempts to locate the user with that username and, if successful, replaces the `~` and the username substring with their home directory (see `man 3 getpwnam`). If `getpwnam` does not locate such a user, the shell leaves the piece unmodified.

### Level 8
- As the shell processes the commands and arguments, if it finds a `<` or `>`, it skips any whitespace characters and attempts to treat the next piece of input as a filename for redirection. If there is no filename before the end of the string, an error message is printed.
- A command can have more than one redirection (even multiple of the same type). If there are multiple of the same redirections, the right-most one takes precedence.
- A redirection beginning with `<` causes the shell to open the corresponding file and replace the `stdin` of the child process with the file descriptor of the open file (see `man 2 dup`).
- A redirection beginning with `>` behaves similarly, but it replaces the `stdout` of the process.

### Level 9
- The shell supports the `|` (pipe) operator to chain multiple commands and their inputs and outputs.
- Each command separated by a `|` is spawned as its own child process. The shell can handle more than one pipe.
- The shell creates a unidirectional pipe (see `man 2 pipe`) for each `|` and redirects the `stdout` from the left command to the writing end of the pipe and redirects the `stdin` of the right command to the reading end of the pipe.
- Any file redirections specified by the user take precedence over any implied redirections of the `|`.

## How to Execute

### Level 0:
1. Open your terminal or PowerShell.
2. Navigate to the directory where your shell levels are stored.
    ```bash
    cd path/to/your/shell_levels
    ```
3. To compile the shell for level 0:
    ```bash
    g++ lvl_0.c -o lvl_0
    ```
4. Execute the shell:
    ```bash
    ./lvl_0
    ```
5. You should see a prompt similar to:
    ```
    /your/current/directory$
    ```

### Level 1:
1. Compile the shell for level 1:
    ```bash
    g++ lvl_1.c -o lvl_1
    ```
2. Execute the shell:
    ```bash
    ./lvl_1
    ```
3. The shell will display the current directory in the prompt. It will keep reading user input until you press `Ctrl+D` (EOF).

### General Steps for Other Levels:
1. To compile any other level, replace `lvl_X.c` with the appropriate level number:
    ```bash
    g++ lvl_X.c -o lvl_X
    ```
2. Run the executable:
    ```bash
    ./lvl_X
    ```
3. The shell will behave according to the functionality of that level.
4. Continue this process for each subsequent level by compiling the respective `lvl_X.c` file and running the executable.

For example:
- Compile and execute for level 2:
    ```bash
    g++ lvl_2.c -o lvl_2
    ./lvl_2
    ```

### Cleaning Up:
- To remove any compiled executables:
    ```bash
    rm lvl_*
    ```

