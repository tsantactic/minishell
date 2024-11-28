/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_with_pipe.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sandriam <sandriam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 10:16:09 by sandriam          #+#    #+#             */
/*   Updated: 2024/11/28 11:51:00 by sandriam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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
        while (cmd->commands_arg[i][j]) 
        {
            free(cmd->commands_arg[i][j]);
            j++;
        }
        free(cmd->commands_arg[i]);
        i++;
    }
}
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
}