/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_command.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sandriam <sandriam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 15:55:10 by tambinin          #+#    #+#             */
/*   Updated: 2024/11/18 10:29:52 by sandriam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
/*test if builtin*/
/*/usr/bin/ ou /bin/ */ 

int contains_bin(char *command)
{
    if (command[0] == '/' && command[1] == 'b' && command[2] == 'i' && command[3] == 'n' && command[4] == '/')
        return (1);
    if (command[0] == '/' && command[1] == 'u' && command[2] == 's' && command[3] == 'r' && command[4] == '/' && command[5] == 'b' && command[6] == 'i' && command[7] == 'n' && command[8] == '/')
        return (1);
    return (0);
}
char *extract_command_bin(char *command)
{
    int is_bin;
    char *result;
    is_bin = 0;
    if (command[0] == '/' && command[1] == 'b' && command[2] == 'i' && command[3] == 'n' && command[4] == '/')
        is_bin = 1;
    if (command[0] == '/' && command[1] == 'u' && command[2] == 's' && command[3] == 'r' && command[4] == '/' && command[5] == 'b' && command[6] == 'i' && command[7] == 'n' && command[8] == '/')
        is_bin = 2;
    if (is_bin == 1)
    {
        result = malloc(sizeof(char) * (ft_strlen(command)-5) + 1);
        size_t i = 0;
        int index = 5;
        while (i < (ft_strlen(command)-5))
        {
            result[i] = command[index];
            i++;
            index++;
        }
        result[i] = '\0';
    }
    else if (is_bin == 2)
    {
        result = malloc(sizeof(char) * (ft_strlen(command)-9) + 1);
        size_t i = 0;
        int index = 9;
        while (i < (ft_strlen(command)-9))
        {
            result[i] = command[index];
            i++;
            index++;
        }
        result[i] = '\0';
    }
    return (result);
}
/*test contains redirection*/
int contains_redirection(char **cmd_arg, int len)
{
    int i = 0;
    while (i < len)
    {
        if (ft_strcmp(cmd_arg[i], "<") == 0)
            return (1);
        if (ft_strcmp(cmd_arg[i], "<<") == 0)
            return (2);
        if (ft_strcmp(cmd_arg[i], ">") == 0)
            return (3);
        if (ft_strcmp(cmd_arg[i], ">>") == 0)
            return (4);
        i++;
    }
    return (0);
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
        printf("\n");
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
        while (cmd->commands_arg[i][j]) 
        {
            free(cmd->commands_arg[i][j]);
            j++;
        }
        free(cmd->commands_arg[i]);
        i++;
    }
}
/*execute with pipe*/ /*si on a ls | ls -a | ls << e on met dans une variable de cmd [ls] dans un autre [ls] [-a] dans un autre [ls]*/
void execute_with_pipes(t_cmd *cmd, char *env[])
{
    (void)env;
    int i = 0, j = 0, k = 0;

    cmd->commands_arg = malloc(sizeof(t_token **) * (cmd->nb_pipe + 1));
    if (!cmd->commands_arg) return;

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
            k++;
        }
        i++;
    }
    cmd->commands_arg[j][k] = NULL;

    print_commands(cmd);
    free_commands(cmd);
    free(cmd->commands_arg);
    free_token_list(cmd);
}

/*execute one commande without pipe and with builtins if is_builtins*/
int is_builtin(char *command)
{
    if (ft_strcmp(command, "cd") == 0)
        return (1);
    if (ft_strcmp(command, "pwd") == 0)
        return (2);
    if (ft_strcmp(command, "export") == 0)
        return (3);
    if (ft_strcmp(command, "unset") == 0)
        return (4);
    if (ft_strcmp(command, "env") == 0)
        return (5);
    if (ft_strcmp(command, "exit") == 0)
        return (6);
    return (0);
}
void execute_all(t_cmd *cmd, char *envp[])
{
    char **my_t_cmd;
    char *path;
    int i = 0;
    int len = 0;
    char *command = NULL;

    while (i < cmd->len_tokens)
    {
        if (cmd->tokens[i]->type != 2)
            len++;
        i++;
    }
    my_t_cmd = malloc(sizeof(char *) * (len + 1));
    if (!my_t_cmd)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    int j = 0;
    i = 0;
    while (i < cmd->len_tokens)
    {
        if (cmd->tokens[i]->type != 2)
        {
            my_t_cmd[j] = ft_strdup(cmd->tokens[i]->value);
            if (!my_t_cmd[j])
            {
                perror("strdup");
                ft_free(my_t_cmd);
                free_token_list(cmd);
                exit(EXIT_FAILURE);
            }
            j++;
        }
        if (cmd->tokens[i]->type == 1)
            command = cmd->tokens[i]->value;
        i++;
    }
    my_t_cmd[j] = NULL;
    if (command && is_builtin(command))
    {
        redir_exec(cmd, envp);
        return ;
    }
    else 
    {
        pid_t pid;

        pid = fork();
        if (pid == -1)
            ft_perror("fork");
        if (pid == 0)
        {
            if (contains_redirection(my_t_cmd, len))
            {
                for (int k = 0; k < j; k++)
                    free(my_t_cmd[k]);
                free(my_t_cmd);
                redir_exec(cmd, envp);
                return ;
            }
            else if (command && !is_builtin(command) && !contains_bin(command) && !contains_redirection(my_t_cmd, len))
            {
                path = ft_find_path(command, envp);
                ft_execute_command(cmd, path, my_t_cmd, envp);
                exit(0);
            }
            else if (command && contains_bin(command) && !contains_redirection(my_t_cmd, len))
            {
                write(2,"\n*is chemin absolute*\n",22);
                command = extract_command_bin(command);
                path = ft_find_path(command, envp);
                free(command);
                ft_execute_command(cmd, path, my_t_cmd, envp);
                exit(1);
            }
        }
        else
        {
            if (waitpid(pid, NULL, 0) == -1)
                ft_perror("waitpid");
            free_token_list(cmd);
        }
   }
   ft_free(my_t_cmd);
}
