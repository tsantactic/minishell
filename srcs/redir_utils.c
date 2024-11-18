/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sandriam <sandriam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 09:48:27 by sandriam          #+#    #+#             */
/*   Updated: 2024/11/18 18:40:59 by sandriam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void loop_heredoc(char *delimiter, int *pipefd)
{
    char *input = NULL;
    while (1)
    {
        input = readline("heredoc>");
        if (!input)
            break;
        if (ft_strcmp(input, delimiter) == 0)
        {
            free(input);
            close(pipefd[1]);
            break;
        }
        ft_putendl_fd(input, pipefd[1]);
        free(input);
    }
}
char  *copy_command_arg(t_cmd *cmd, char **my_t_cmd, char *command)
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
    return(command);
}
