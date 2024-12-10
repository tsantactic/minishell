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
    int stdout_backup = dup(STDOUT_FILENO);
    int stdin_backup = dup(STDIN_FILENO);
    while (1) {
        input = readline(">");
        if (strcmp(input, "exit") == 0) {
            dup2(stdout_backup, STDOUT_FILENO);
            dup2(stdin_backup, STDIN_FILENO);
            close(stdout_backup);
            close(stdin_backup);
            exit(set_nb(12));
        }
        if (strcmp(input, "success") == 0) {
            // pid_t pid = fork();
            // if (pid == 0) {
                char *arg[] = { NULL, "ls", "-a", NULL };
                execve("/bin/ls", arg, NULL );
                perror("test:");
        //         g_i = 1;
        //         exit(set_nb(g_i));
        //     } else {
        //         waitpid(pid, NULL, 0);
        //     }
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
// #include <signal.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <stdio.h>
// #include <string.h>
// #include <sys/wait.h>
// #include <readline/readline.h>

// void set_st(int status);
// char *expand_heredoc(char *input, t_cmd *cmd);

// int set_sig_heredoc(int nb)
// {
//     static int i;
//     if (nb >= 0)
//         i = nb;
//     return (i);
// }

// void sig_handler_heredoc(int signal)
// {
//     if (signal == SIGINT)
//     {
//         set_st(130);
//         set_sig_heredoc(1);
//     }
// }

// void loop_heredoc(char *delimiter, int *pipefd, t_cmd *cmd)
// {
//     char *input = NULL;

//     set_sig_heredoc(0);
//     signal(SIGINT, sig_handler_heredoc);

//     while (1)
//     {
//         input = readline("heredoc> ");
//         if (!input)
//         {
//             fprintf(stderr, "minishell: warning: here-document at this line delimited by end-of-file (wanted '%s')\n", delimiter);
//             close(pipefd[1]);
//             set_st(0);
//             break;
//         }
//         if (set_sig_heredoc(-1) == 1)
//         {
//             set_sig_heredoc(0);
//             free(input);
//             close(pipefd[1]);
//             break;
//         }
//         if (strcmp(input, delimiter) == 0)
//         {
//             free(input);
//             close(pipefd[1]);
//             set_st(0);
//             break;
//         }
//         input = expand_heredoc(input, cmd);
//         dprintf(pipefd[1], "%s\n", input);
//         free(input);
//     }
// }

// void execute_heredoc(t_cmd *cmd, int **pipe_heredoc, int count_heredoc)
// {
//     int heredoc_index = 0;
//     int i = 0;
//     char *delimiter;

//     while (i < cmd->len_tokens)
//     {
//         if (cmd->tokens[i]->type == REDIR_HEREDOC)
//         {
//             if (cmd->tokens[i + 1]->type == DELIMITER)
//             {
//                 delimiter = cmd->tokens[i + 1]->value;
//                 pipe(pipe_heredoc[heredoc_index]);

//                 pid_t pid = fork();
//                 if (pid == 0) // Processus fils
//                 {
//                     close(pipe_heredoc[heredoc_index][0]); // Fermer le côté lecture
//                     loop_heredoc(delimiter, pipe_heredoc[heredoc_index], cmd);
//                     exit(EXIT_SUCCESS); // Quitter proprement le processus fils
//                 }
//                 else if (pid > 0) // Processus parent
//                 {
//                     close(pipe_heredoc[heredoc_index][1]); // Fermer le côté écriture
//                     int status;
//                     waitpid(pid, &status, 0); // Attendre la fin du processus fils
//                     if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
//                     {
//                         set_st(130); // Signal d'interruption (Ctrl+C)
//                         break;
//                     }
//                     if (WIFEXITED(status))
//                         set_st(WEXITSTATUS(status));
//                 }
//                 else
//                 {
//                     perror("fork");
//                     exit(EXIT_FAILURE);
//                 }
//                 heredoc_index++;
//             }
//             i += 1;
//         }
//         i++;
//     }
// }
