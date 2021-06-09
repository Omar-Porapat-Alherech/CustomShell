# CustomShell

Uses two previously created data structures - Included it here to avoid importing the previous iteration.

**Main.c** 
A shell implementation, demonstrating the use of forks and pipes complete certain commands fed into the shell. Supports most commands as executed by execvp, but this code handles the piping / input / printing / output from child processes back to the parent.

**get_char_put_char**
An implementation of get_char and put_char using x86_asm, demonstrating the use of Inline assembly within c code.

**ls_implementation** 
Implementation of the "ls" command - List. Prints the names of directories and files in the current working directory. Also allows the use of the "cd" command - Change Directory.
