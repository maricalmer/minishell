# Minishell

## Mandatory part

- [x] 1. **Prompt Display**: Displays a prompt `minishell` and waits for user input.

- [x] 2. **Command History**:
   - minishell maintains a history of commands entered by the user.

- [x] 3. **Command Execution**:
   - minishell searches for and executes the correct executable based on the `PATH` environment variable or using a relative or absolute path.
   - minishell does not need to handle unclosed quotes or special characters like `\` or `;`.

- [x] 4.  **Quote Handling**:
   - **Single Quotes (`'`)**: Prevent the shell from interpreting meta-characters within the quotes.
   - **Double Quotes (`"`):** Prevent the shell from interpreting meta-characters within the quotes except for the `$` character.

- [x] 5.  **Redirections**:
   - Input Redirection (`<`): Redirects input from a file.
   - Output Redirection (`>`): Redirects output to a file.
   - Append Output Redirection (`>>`): Appends output to the end of a file.
   - Heredoc (`<<`): Reads input until a delimiter is found.

- [x] 6.  **Pipes (`|`)**:
   - minishell supports pipelines, allowing the output of one command to be the input of another.

- [x] 7.  **Environment Variable Expansion**:
   - [x] minishell expands environment variables prefixed with `$` followed by a sequence of characters into their corresponding values.
   - [x] minishell handles $? which should expand to the exit status of the most recently executed foreground pipeline

- [x] 8.  **Signal Handling**:
   - minishell handles signals like `Ctrl+C`, `Ctrl+D`, and `Ctrl+\` appropriately, using at most one global variable for signal handling.
   - [x]  **`Ctrl+C`** (`SIGINT`): Displays a new prompt on a new line.
   - [x]  **`Ctrl+D`**: Sends an EOF character which can be detected as an empty input from `readline`. Exits the shell.
   - [x]  **`Ctrl+\`** (`SIGQUIT`): Does nothing.

- [x] 9.  **Built-in Commands**:
   - [x] `echo` with `-n` option: Displays a line of text.
   - [x] `cd` with a relative or absolute path: Changes the current working directory.
   - [x] `pwd`: Prints the current working directory.
   - [x] `export`: Sets environment variables.
   - [x] `unset`: Unsets environment variables.
   - [x] `env`: Displays the environment variables.
   - [x] `exit`: Exits the shell.

- [x] 10. **Memory Management**:
    - The shell handles memory allocation and deallocation properly. Memory leaks from the `readline` function are acceptable and do not need to be fixed.
	 - suppressions.supp file is currently supressing the memory leaks caused by `readline()`. To use it with valgrind, the command is:
	```bash
	 valgrind --suppressions=suppressions.supp --leak-check=full --show-leak-kinds=all ./minishell
	```
	- If there are no other leaks from other functions other than `readline()`, the output after `exit` is going to be something like:
	```bash
	==00001== LEAK SUMMARY:
	==00001==    definitely lost: 0 bytes in 0 blocks
	==00001==    indirectly lost: 0 bytes in 0 blocks
	==00001==      possibly lost: 0 bytes in 0 blocks
	==00001==    still reachable: 0 bytes in 0 blocks
	==00001==         suppressed: 204,161 bytes in 221 blocks
	```

## Allowed Functions

  - **Standard C library**: `malloc`, `free`, `write`, `read`, `close`.
  - **Input/Output**: `readline`, `rl_clear_history`, `rl_on_new_line`, `rl_replace_line`, `rl_redisplay`, `add_history`, `printf`, `perror`, `strerror`, `isatty`, `ttyname`, `ttyslot`, `tgetent`, `tgetflag`, `tgetnum`, `tgetstr`, `tgoto`, `tputs`.
  - **Process Control**: `fork`, `wait`, `waitpid`, `wait3`, `wait4`, `kill`, `exit`, `execve`.
  - **Signal Handling**: `signal`, `sigaction`, `sigemptyset`, `sigaddset`.
  - **File Operations**: `open`, `access`, `stat`, `lstat`, `fstat`, `unlink`, `opendir`, `readdir`, `closedir`, `dup`, `dup2`, `pipe`.
  - **Environment**: `getenv`, `tcsetattr`, `tcgetattr`.
  - **Directory**: `chdir`, `getcwd`.
  - **Terminal Control**: `ioctl`.

---
## Github collaboration
**For clarity, in the commit history, we use the following prefixes**

  - `feat:` for new features and major functionalities.
  - `fix:` for bug fixes or corrections.
  - `docs:` for documentation updates.
  - `style:` for code style improvements and refactoring.

### Git Workflow
1. **Create a New Branch:**
    ```bash
    git checkout -b branch-name
    ```

2. **Push New Branch to GitHub:**
    ```bash
    git push -u origin branch-name
    ```

3. **Stay Updated with Main Branch:**
    ```bash
    git checkout main
    git pull
    git checkout branch-name
    git merge main
    ```

4. **Merge Branch into Main:**
    ```bash
    git checkout main
    git merge branch-name
    git push
    ```

5. **Delete a Branch:**
    ```bash
    git branch -d branch-name
    git push origin --delete branch-name
    ```

- **Resolve Conflicts:** If there are any merge conflicts during the `git merge` step, Git will notify you. You'll need to manually resolve these conflicts in the affected files, then add and commit the resolved files.

---
### Common Modules

1. **Main Loop**: Core loop handling prompt display and user input.
2. **Input Handling**: Reading and preliminary processing of input, including checking for unclosed quotes.
3. **Lexer/Tokenizer**: Breaking input into tokens (words and operators).
4. **Parser**: Constructing command structures from tokens, handling syntax rules.
5. **Expander**: Handling environment variable expansion and wildcard expansion.
6. **Executor**: Executing commands, managing built-ins and external commands.
7. **Built-in Commands**: Implementing the required built-in functions.
8. **Environment Management**: Handling environment variables and their manipulation.
9. **Redirections**: Managing input/output redirections and heredocs.
10. **Pipes**: Handling inter-process communication via pipes.
11. **Signal Handling**: Managing signals (`ctrl-C`, `ctrl-D`, `ctrl-\`).
12. **History Management**: Implementing command history using the `readline` library.
13. **Utilities**: Helper functions and common utilities.
14. **Error Handling**: Providing user-friendly error messages and handling exit statuses.
15. **Memory Management**: Ensuring all allocated memory is properly freed.

---
### General Resources

1. **Student Project Documentation**:
	- [Minishell by Maia de Graaf and Alfred Polycarpe](https://github.com/maiadegraaf/minishell): Provided insights into project structuring and implementation details.
	- [Minishell by Vportens](https://github.com/vportens/minishell): Offered a breakdown of parsing, built-ins, redirections, and execution.
2. **Books**:
	- *Advanced Programming in the UNIX Environment* by W. Richard Stevens: Comprehensive resource on Unix system calls and process management.
	- *The Linux Programming Interface* by Michael Kerrisk: Detailed explanations of Linux system programming concepts.

