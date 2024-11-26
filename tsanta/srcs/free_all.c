/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_all.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sandriam <sandriam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 09:00:39 by sandriam          #+#    #+#             */
/*   Updated: 2024/11/26 11:11:57 by sandriam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void free_and_reinitialise(t_cmd *cmd)
{
    int i;
    i = 0;
    while (cmd->args[i] != NULL)
        free(cmd->args[i++]);
    memset(cmd->args, 0, sizeof(char *) * INITIAL_ARG_SIZE);
    cmd->arg_index = 0;
}
void	free_tokens(t_cmd *cmd)
{
	int	i;

	i = 0;
	while (i < cmd->len_tokens)
	{
		free(cmd->tokens[i]->value);
		free(cmd->tokens[i]);
		i++;
	}
	free(cmd->tokens);
}

void free_tokens_all(t_cmd *cmd)
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
    j = 0;
    while (cmd->args[j])
    {
        free(cmd->args[j]);
        j++;
    }
    free_tokens(cmd);
}