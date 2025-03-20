#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_INPUT 1024
#define MAX_ARGS 64
#define HISTORY_SIZE 10

char *history[HISTORY_SIZE];
int history_count = 0;

void add_to_history(char *cmd) {
    if (history_count < HISTORY_SIZE) {
        history[history_count++] = strdup(cmd);
    } else {
        free(history[0]);
        for (int i = 1; i < HISTORY_SIZE; i++) {
            history[i - 1] = history[i];
        }
        history[HISTORY_SIZE - 1] = strdup(cmd);
    }
}

void print_history() {
    for (int i = 0; i < history_count; i++) {
        printf("%d: %s\n", i + 1, history[i]);
    }
}

void execute_command(char **args) {
    pid_t pid = fork();
    if (pid == 0) { // Child process
        execvp(args[0], args);
        perror("exec");
        exit(EXIT_FAILURE);
    } else if (pid > 0) { // Parent process
        wait(NULL);
    } else {
        perror("fork");
    }
}

void execute_piped_command(char **args1, char **args2) {
    int pipefd[2];
    pipe(pipefd);
    pid_t pid1 = fork();

    if (pid1 == 0) { // First child
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
        execvp(args1[0], args1);
        perror("exec1");
        exit(EXIT_FAILURE);
    }

    pid_t pid2 = fork();
    if (pid2 == 0) { // Second child
        close(pipefd[1]);
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);
        execvp(args2[0], args2);
        perror("exec2");
        exit(EXIT_FAILURE);
    }

    close(pipefd[0]);
    close(pipefd[1]);
    wait(NULL);
    wait(NULL);
}

void parse_and_execute(char *input) {
    add_to_history(input);
    char *args[MAX_ARGS];
    char *token = strtok(input, " ");
    int i = 0;

    while (token != NULL) {
        args[i++] = token;
        token = strtok(NULL, " ");
    }
    args[i] = NULL;

    if (i == 0)
        return;
    if (strcmp(args[0], "exit") == 0)
        exit(0);
    if (strcmp(args[0], "history") == 0) {
        print_history();
        return;
    }

    // Handle pipes
    char *args1[MAX_ARGS], *args2[MAX_ARGS];
    int pipe_index = -1;
    for (int j = 0; j < i; j++) {
        if (strcmp(args[j], "|") == 0) {
            pipe_index = j;
            break;
        }
    }
    if (pipe_index != -1) {
        memcpy(args1, args, pipe_index * sizeof(char *));
        args1[pipe_index] = NULL;
        memcpy(args2, &args[pipe_index + 1],
               (i - pipe_index - 1) * sizeof(char *));
        args2[i - pipe_index - 1] = NULL;
        execute_piped_command(args1, args2);
    } else {
        execute_command(args);
    }
}

int main() {
    char input[MAX_INPUT];
    while (1) {
        printf("mysh> ");
        if (!fgets(input, MAX_INPUT, stdin))
            break;
        input[strcspn(input, "\n")] = 0; // Remove newline
        parse_and_execute(input);
    }
    return 0;
}