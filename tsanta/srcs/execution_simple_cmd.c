/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_simple_cmd.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sandriam <sandriam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 10:26:02 by sandriam          #+#    #+#             */
/*   Updated: 2024/11/26 11:56:07 by sandriam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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

char *set_command(t_cmd *cmd, char *command)
{
    int i = 0;
    while (i < cmd->len_tokens)
    {
        if (cmd->tokens[i]->type == 1)
            command = cmd->tokens[i]->value;
        i++;
    }
    return (command);
}

void ft_free_token_cmd(t_cmd *cmd)
{
    int	j;

    j = 0;
    while (j < cmd->len_arg)
    {
        free(cmd->token_arg[j]);
        j++;
    }
    free(cmd->token_arg);
    cmd->token_arg = NULL;
    ft_free(cmd->args);
}
void execute_without_pipe(t_cmd *cmd, char **env)
{
    char *command = NULL;

    command = set_command(cmd, command);
    if (command && is_builtin(command))
        parsing_arg_with_builtins(cmd, env); /***** parse and execution with builtins ***** */
    else
    {
        pid_t pid = fork();
        if (pid == 0)
        {
            parsing_argument_simple(cmd, env); /***** parse and execution simple command ***** */
            ft_free_token_cmd(cmd);
            exit (set_state(-1));
        }
        else
        {
            waitpid(pid, NULL, 0);
        }
    }
    return;
}
