/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_with_pipe.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sandriam <sandriam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 10:16:09 by sandriam          #+#    #+#             */
/*   Updated: 2024/12/10 17:52:34 by sandriam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void count_cmd_per_arg(t_cmd *cmd)
{
    size_t i = 0;
    int len = 0;
    int j = 0;
    while (i <= cmd->nb_pipe)
    {
        j = 0;
        len = 0;
        printf("%ld command: ", i);
        while (cmd->commands_arg[i][j] && cmd->commands_arg[i][j]->value)
        {
            if (cmd->commands_arg[i][j]->type == ARG || cmd->commands_arg[i][j]->type == CMD)
            {
                if (cmd->commands_arg[i][j]->type == CMD)
                    cmd->commands_arg[i][0]->command_value = ft_strdup(cmd->commands_arg[i][0]->value);
                len++;
            }
            j++;
        }
        cmd->commands_arg[i][0]->len_cmd_arg = len;
       
        cmd->commands_arg[i][0]->tmp_cmd_arg = malloc(sizeof(char *) * (cmd->commands_arg[i][0]->len_cmd_arg + 1));
        j = 0;
        int k = 0;
        while (cmd->commands_arg[i][j] && cmd->commands_arg[i][j]->value)
        {
            if (cmd->commands_arg[i][j]->type == ARG || cmd->commands_arg[i][j]->type == CMD)
            {
                cmd->commands_arg[i][0]->tmp_cmd_arg[k++] = ft_strdup(cmd->commands_arg[i][j]->value);
            }
            j++;
        }
        cmd->commands_arg[i][0]->tmp_cmd_arg[k] = NULL;
        printf("dans %ld nous avons %d[%d] nombre de commande et son command principal est %s\n voici les commande apres parse: \n", i, len, cmd->commands_arg[i][0]->len_cmd_arg = len, cmd->commands_arg[i][0]->command_value);
        k = 0;
        while (k < cmd->commands_arg[i][0]->len_cmd_arg)
        {
            printf("[%s] ", cmd->commands_arg[i][0]->tmp_cmd_arg[k]);
            k++;
        }
        printf("\n");
        i++;
    }
}

void execute_piped_commands(t_cmd *cmd, t_env **env)
{
    size_t i;
    int pipe_fd[2 * cmd->nb_pipe];
    pid_t pid;

    for (i = 0; i < cmd->nb_pipe; i++)
    {
        if (pipe(pipe_fd + i * 2) == -1)
        {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }

    for (i = 0; i <= cmd->nb_pipe; i++)
    {
        pid = fork();
        if (pid == -1)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0)
        {
            
            if (i > 0)
            {
                if (dup2(pipe_fd[(i - 1) * 2], STDIN_FILENO) == -1)
                {
                    perror("dup2 input");
                    exit(EXIT_FAILURE);
                }
            }
            
            if (i < cmd->nb_pipe) {
                if (dup2(pipe_fd[i * 2 + 1], STDOUT_FILENO) == -1)
                {
                    perror("dup2 output");
                    exit(EXIT_FAILURE);
                }
            }

            for (size_t j = 0; j < 2 * cmd->nb_pipe; j++)
            {
                close(pipe_fd[j]);
            }
            char *path = ft_find_path(cmd->commands_arg[i][0]->command_value, env);
            if (execve(path, cmd->commands_arg[i][0]->tmp_cmd_arg, NULL) == -1)
            {
                perror("execvp");
                exit(EXIT_FAILURE);
            }
        }
    }

    for (i = 0; i < 2 * cmd->nb_pipe; i++)
    {
        close(pipe_fd[i]);
    }

    for (i = 0; i <= cmd->nb_pipe; i++)
    {
        wait(NULL);
    }
}

void print_commands(t_cmd *cmd)
{
    size_t i = 0;
    int j = 0;

    printf("Total pipes: %ld\n", cmd->nb_pipe);
    while (i <= cmd->nb_pipe)
    {
        j = 0;
        printf("%ld command: ", i);
        while (cmd->commands_arg[i][j] && cmd->commands_arg[i][j]->value)
        {
            printf("%s (type %d)", cmd->commands_arg[i][j]->value,cmd->commands_arg[i][j]->type);
            j++;
        }
        if (i == cmd->nb_pipe)
            printf("\nfd is stdout\n");    
        else
            printf("\nfd is stdin\n");
        i++;
    }
}

void free_commands(t_cmd *cmd)
{
    size_t i = 0;
    int j = 0;

    while (i <= cmd->nb_pipe)
    {
        j = 0;
        if (cmd->commands_arg[i][0]->command_value)
            free(cmd->commands_arg[i][0]->command_value);

        ft_free(cmd->commands_arg[i][0]->tmp_cmd_arg);

        j = 0;
        while (cmd->commands_arg[i][j]) 
        {
            free(cmd->commands_arg[i][j]);
            j++;
        }
        free(cmd->commands_arg[i]);
        i++;
    }
}

void execute_with_pipes(t_cmd *cmd, t_env *env[])
{
    (void)env;
    int i = 0, j = 0, k = 0;

    cmd->commands_arg = malloc(sizeof(t_token **) * (cmd->nb_pipe + 1));
    if (!cmd->commands_arg)
        return ;
    while (i < cmd->len_tokens)
    {
        if (cmd->tokens[i]->type == PIPE)
        {
            cmd->commands_arg[j][k] = NULL;
            j++;
            k = 0;
        }
        else
        {
            if (k == 0)
            {
                cmd->commands_arg[j] = malloc(sizeof(t_token *) * (cmd->len_tokens + 1));
                if (!cmd->commands_arg[j])
                    return;
            }
            cmd->commands_arg[j][k] = malloc(sizeof(t_token));
            if (!cmd->commands_arg[j][k])
                return;

            cmd->commands_arg[j][k]->value = cmd->tokens[i]->value;
            cmd->commands_arg[j][k]->type = cmd->tokens[i]->type;
            cmd->commands_arg[j][k]->type_env = cmd->tokens[i]->type_env;
            cmd->commands_arg[j][k]->type_quote_delim = cmd->tokens[i]->type_quote_delim;
            k++;
        }
        i++;
    }
    cmd->commands_arg[j][k] = NULL;

    print_commands(cmd);
    count_cmd_per_arg(cmd);
    execute_piped_commands(cmd, env);
    free_commands(cmd);
    free(cmd->commands_arg);
}