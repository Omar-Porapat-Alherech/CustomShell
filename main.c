//Arch 5.56 Implementation 4
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <wait.h>
#define STDIN_FILENO 0
#define STDOUT_FILENO 1

#define MAX 1024
void read_and_parse(char *formatted_string, char *user_input, char **argv);
void parse_user_input(char *user_input, char **argv);
char *setup(void);
char **get_params(char **args);
void handle_pipe(char **pleft, char **pright);
void print_output(char **args);
char *substring(char *temp, size_t n);
void handle_input(char **left, char **right);
void handle_output(char **left, char **right);
char *trimwhitespace(char *str);
void execute_argv(char **commands);

int main() {
    char user_input[MAX];
    int pid;
    int status;
    char *argv[64];
    char *commands[64];
    struct Node *head = NULL;
    const char *invalid = "|><&";
    char **params;
    int counter = 1;
    char **arr_pointer = malloc(64 * (sizeof(char *)));
    char *formatted_string = setup();
    // CREATE PIPE
    int pipefd[2];
    pipe(pipefd);
    bool pipe_input_ready = false;
    bool triggered = false;
    while (1) {
        read_and_parse(formatted_string, user_input, argv);
        if (strcmp(argv[0], "exit") == 0) {
            break;
        }
        if (strcmp(argv[0], "cd") == 0) {
            chdir(argv[1]);
            continue;
        }
        params = get_params(argv);
        print_output(argv);
        pid = fork();
        if (pid != 0) {
            /* Parent Code */
            /* Wait for Child to Exit */
            //        params = print_output(commands, head);
            if (triggered == true){
              triggered = false;
            }
            else {
              wait(&status);
            }
        } else {
            while (*params != NULL) {
                if (strchr(invalid, **params)) {
                    if (*(params + 1 + counter) == NULL) {
                        if (**(params) == '&') {
                            triggered = true;
                            execute_argv(params - 1);
                        }
                        if (**params == '<') {
                            handle_input(params - 1, params + 1);
                        }
                        if (**params == '>') {
                            handle_output(params - 1, params + 1);
                        }
                        if (**params == '|') {
                            handle_pipe(params - 1, params + 1);
                        }
                    }
                    counter++;
                }
                *params++;
                if (*params == NULL){
                    execute_argv(params-counter);
                }
            }
        }
    }
}

void handle_pipe(char **pleft_arr, char **pright) {
    int pipefd[2];
    pipe(pipefd);
    int pid;
    int status;
    char *left[64];
    char *right[64];

    int i = 0;
    // refactor left
    left[i] = strtok(*pleft_arr, " ");
    while (left[i] != NULL) {
        left[++i] = strtok(NULL, " ");
    }
    i = 0;
    right[i] = strtok(*pright, " ");
    while (right[i] != NULL) {
        right[++i] = strtok(NULL, " ");
    }

    pid = fork();
    if (pid == 0) {
        dup2(pipefd[1], 1);
        close(pipefd[1]);
        execvp(left[0], left);

    } else {
        close(pipefd[1]);
        wait(&status);
        dup2(pipefd[0], 0);
        execvp(right[0], right);
    }
}

void execute_argv(char **commands) {
    int pid;
    int status;
    char *command_arr[64];
    int i = 0;
    // refactor left
    command_arr[i] = strtok(*commands, " ");
    while (command_arr[i] != NULL) {
        command_arr[++i] = strtok(NULL, " ");
    }
    execvp(command_arr[0], command_arr);
}
void print_output(char **args) {
    const char *invalid = "|><&";
    char **return_val = malloc(64 * sizeof(char *));
    char **token = return_val;
    char **commands = malloc(64 * sizeof(char *));
    char **command_list = commands;
    char **temp = command_list;
    char final_string[MAX] = "";
    char concat[MAX] = "";
    *commands++ = *args;
    while (*args != NULL) {
        if (!strchr(invalid, **args)) {
            strcat(concat, *args);
            strcat(concat, " ");
        } else {
            if (**args == '<' || **args == '>') {
                strcat(concat, *args);
                strcat(concat, " ");
                *args++;
                if (*args != NULL) {
                    strcat(concat, *args);
                    strcat(concat, " ");
                }
                *args++;
                continue;
            } else if (**args == '|') {
                *args++;
                if (*args != NULL) {
                    *commands = (char *)malloc(MAX);
                    strcpy(*commands++, *args);
                    strcat(concat, "\0");
                    *return_val = (char *)malloc(MAX);
                    strcpy(*return_val++, concat);
                    memset(concat, 0, sizeof(concat));
                    strcat(concat, *args);
                    strcat(concat, " ");
                }
            } else if (**args == '&') {
                strcat(concat, *args);
                strcat(concat, " ");
                *args++;
                if (*args != NULL) {
                    *commands = (char *)malloc(MAX);
                    strcpy(*commands++, *args);
                    strcat(concat, "\0");
                    *return_val = (char *)malloc(MAX);
                    strcpy(*return_val++, concat);
                    memset(concat, 0, sizeof(concat));
                    strcat(concat, *args);
                    strcat(concat, " ");
                }
            }
        }
        *args++;
    }
    *return_val = (char *)malloc(MAX);
    strcpy(*return_val, concat);
    fputs("Commands: ", stdout);
    while (*command_list != NULL) {
        fputs(*command_list++, stdout);
        fputs(" ", stdout);
    }
    fputs("\n", stdout);
    command_list = temp;
    while (*token != NULL) {
        sprintf(final_string, "%s:%s\n", *command_list,
                substring(*token, strlen(*command_list)));
        fputs(final_string, stdout);
        *token++;
        *command_list++;
    }
    fputs("\n", stdout);
}

char *trimwhitespace(char *str) {
    char *end;
    while (isspace((unsigned char)*str)) str++;

    if (*str == 0)  // All spaces?
        return str;
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;

    end[1] = '\0';
    return str;
}

char *substring(char *temp, size_t n) {
    char *str = temp;
    size_t len = strlen(str);
    if (n > len) return str;  // Or: n = len;
    for (int i = 0; i < n; i++) {
        str++;
    }
    return str;
}
char **get_params(char **args) {
    const char *invalid = "|><&";
    char **return_val = malloc(64 * sizeof(char *));
    char **token = return_val;
    char *arr_pointers[64];
    char concat[MAX] = "";
    while (*args != NULL) {
        *return_val = (char *)malloc(MAX);
        if (!strchr(invalid, **args)) {
            strcat(concat, *args);
            strcat(concat, " ");
        } else {
            strcat(concat, "\0");
            strcpy(*return_val++, concat);
            *return_val = (char *)malloc(MAX);
            strcpy(*return_val++, *args);
            memset(concat, 0, sizeof(concat));
        }
        *args++;
    }
    *return_val = (char *)malloc(MAX);
    strcpy(*return_val, trimwhitespace(concat));
    return token;
}


void read_and_parse(char *formatted_string, char *user_input, char **argv) {
    fputs(formatted_string, stdout);
    fgets(user_input, MAX, stdin);
    // Skipline
    fputs("\n", stdout);
    parse_user_input(user_input, argv);
}

void parse_user_input(char *user_input, char **argv) {
    bool mid_string = false;
    while (*user_input != '\0') {
        while (*user_input == ' ' || *user_input == '\t' || *user_input == '\n') {
            if (!mid_string) {
                *user_input++ = '\0';
            } else {
                *user_input++;
            }
        }
        if (!mid_string && *user_input != '\0') {
            *argv++ = user_input;
        }
        while (*user_input != '\0' && *user_input != ' ' && *user_input != '\t' &&
               *user_input != '\n') {
            if (*user_input == '\"' || *user_input == '\'') {
                mid_string = !mid_string;
            }
            user_input++;
        }
    }
    *argv = '\0';
}

char *setup(void) {
    char *name = getenv("LOGNAME");
    char *cwd = getenv("PWD");
    char *Formatted = malloc(strlen(name) + strlen(cwd) + 17);
    // Move Pointer to remove /home/username
    cwd += (strlen(getenv("OLDPWD")));
    // 18 Including null terminators and characters manually added
    // Trying out manual mallocing
    sprintf(Formatted, "λ %s@ESE333 [%s] \n→ ", name, cwd);
    return Formatted;
}


void handle_ampersand(char **left) {
    execute_argv(**left);
    // No need to waitpid
}
void handle_output(char **left, char **right) {
    int i = 0;
    // refactor left
    char *lefts[64];
    lefts[i] = strtok(*left, " ");
    while (lefts[i] != NULL) {
        lefts[++i] = strtok(NULL, " ");
    }
    int out = open(*right, O_WRONLY | O_TRUNC | O_CREAT,
                   S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
    dup2(out, STDOUT_FILENO);
    close(out);
    execvp(lefts[0], lefts);
    // No need to waitpid
}
void handle_input(char **left, char **right) {
    int new_file = open(*right, O_RDONLY);
    int i = 0;
    char *lefts[64];
    lefts[i] = strtok(*left, " ");
    while (lefts[i] != NULL) {
        lefts[++i] = strtok(NULL, " ");
    }
    dup2(new_file, STDIN_FILENO);
    execvp(lefts[0], lefts);
}




