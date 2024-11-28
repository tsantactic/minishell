#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <readline/readline.h>

int g_i = 0; // Variable globale pour le suivi

int set_nb(int nb) {
    if (nb >= 0) {
        g_i = nb;
    }
    return g_i;
}

void ft_readline(char **env) {
    char *input;
    while (1) {
        input = readline(">");
        if (strcmp(input, "exit") == 0) {
            exit(set_nb(12));
        }
        if (strcmp(input, "success") == 0) {
            pid_t pid = fork();
            if (pid == 0) {
                char *arg[] = { NULL, "ls", "-a", NULL };
                execve("/bin/ls", arg, env);
                perror("test:");
                g_i = 1;
                exit(set_nb(g_i));
            } else {
                waitpid(pid, NULL, 0);
            }
        }
        if (strcmp(input, "fail") == 0) {
            set_nb(2);
        }
        printf("%d\n", set_nb(-1));
        free(input);
    }
}

int main(int arg, char **argv, char **env) {
    (void)arg;
    (void)argv;
    printf("entrer votre entree:\n");
    ft_readline(env);
    return 0;
}