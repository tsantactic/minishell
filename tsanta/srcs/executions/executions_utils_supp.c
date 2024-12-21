/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executions_utils_supp.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tambinin <tambinin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 11:40:16 by sandriam          #+#    #+#             */
/*   Updated: 2024/12/20 13:47:19 by tambinin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/****command****/
void count_command_arg_blt(t_cmd *cmd, int *len_command)
{
    int i;
    i = 0;
    while (i < cmd->len_tokens)
    {
        if (cmd->tokens[i]->type == CMD || cmd->tokens[i]->type == ARG)
        {
            (*len_command)++;
        }
        i++;
    }
}

void count_command_arg(t_cmd *cmd, int *len_command)
{
    int i;
    i = 0;
    while (i < cmd->len_tokens)
    {
        if (cmd->tokens[i]->type == CMD || cmd->tokens[i]->type == ARG)
        {
            if (cmd->tokens[i]->type_env == IS_ENV && ft_strchr(cmd->tokens[i]->value, ' ') != 0)
            {
                char **split_env = ft_split(cmd->tokens[i]->value, ' ');
                int count = 0;
                while (split_env && split_env[count])
                {
                    free(split_env[count]);
                    count++;
                }
                free(split_env);
                (*len_command) += count;
            }
            else
                (*len_command)++;
        }
        i++;
    }
}
char *copy_command_arg_blt(t_cmd *cmd, char **my_t_cmd, char *command)
{
    int j = 0;
    int k = 0;
    while (k < cmd->len_tokens)
    {
        if (cmd->tokens[k]->type == ARG || cmd->tokens[k]->type == CMD)
        {
            if (cmd->tokens[k]->type == CMD)
                command = ft_strdup(cmd->tokens[k]->value);
            my_t_cmd[j++] = ft_strdup(cmd->tokens[k]->value);
        }
        k++;
    }
    my_t_cmd[j] = NULL;
    return (command);
}

char *copy_command(t_cmd *cmd, char *command)
{
    int k = 0;
    while (k < cmd->len_tokens)
    {
        if (cmd->tokens[k]->type == ARG || cmd->tokens[k]->type == CMD)
        {
            if (cmd->tokens[k]->type == CMD)
                command = ft_strdup(cmd->tokens[k]->value);
        }
        k++;
    }
    return (command);
}
char *copy_command_arg(t_cmd *cmd, char **my_t_cmd, char *command)
{
    int j = 0;
    int k = 0;
    while (k < cmd->len_tokens)
    {
        if (cmd->tokens[k]->type == ARG || cmd->tokens[k]->type == CMD)
        {
            if (cmd->tokens[k]->type_env == IS_ENV && ft_strchr(cmd->tokens[k]->value, ' ') != 0)
            {
                char **split_env = ft_split(cmd->tokens[k]->value, ' ');
                int i = 0;
                while (split_env && split_env[i])
                {
                    my_t_cmd[j++] = ft_strdup(split_env[i]);
                    if (i == 0)
                        command = ft_strdup(split_env[0]);
                    free(split_env[i]);
                    i++;
                }
                i = 0;
                free(split_env);
            }
            else
            {
                if (cmd->tokens[k]->type == CMD)
                    command = ft_strdup(cmd->tokens[k]->value);
                my_t_cmd[j++] = ft_strdup(cmd->tokens[k]->value);
            }
        }
        k++;
    }
    my_t_cmd[j] = NULL;
    return (command);
}
int contains_bin(char *command)
{
    if ((ft_strncmp(command, "/bin/", 5) == 0 || ft_strncmp(command, "/usr/bin/", 9) == 0))
    {
        return(1);
    }
    return (0);
}

char *extract_command_bin(char *command)
{
    if (contains_bin(command))
    {
        if (ft_strncmp(command, "/bin/", 5) == 0)
        {
            return (command + 5);
        }
        else if (ft_strncmp(command, "/usr/bin/", 9) == 0)
        {
            return (command + 9);
        }
    }
    return ft_strdup(command);
}

int check_env(t_env **envp, char *value)
{
    t_env *curr = NULL;

    curr = *envp;
    while (curr)
    {
        if (ft_strncmp(curr->data, value, ft_strlen(value)) == 0)
        {
            return (1);
        }
        curr = curr->next;
    }
    return (0);
}
