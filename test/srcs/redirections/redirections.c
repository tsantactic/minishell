/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tambinin <tambinin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 10:43:42 by sandriam          #+#    #+#             */
/*   Updated: 2024/12/24 08:43:06 by tambinin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	parse_exec_redirection(t_cmd *cmd)
{
	if (contains_redirection(cmd))
	{
		if (redirection_exec(cmd) == 2)
		{
			if (contains_heredoc(cmd))
				free_pipe_heredoc(cmd->count_heredoc, cmd->pipe_heredoc);
			ft_free_token_cmd(cmd);
			free_tokens(cmd);
			free_new_env(cmd->env_lst);
			exit(1);
		}
	}
}

int	contains_redirection(t_cmd *cmd)
{
	int	i;

	i = 0;
	while (i < cmd->len_arg)
	{
		if (cmd->tokens[i]->type == REDIR_IN)
			return (1);
		if (cmd->tokens[i]->type == REDIR_OUT)
			return (1);
		if (cmd->tokens[i]->type == REDIR_OUTPUT_APPEND)
			return (1);
		i++;
	}
	return (0);
}

int	redirection_exec(t_cmd *cmd)
{
	int	k;

	k = 0;
	while (k < cmd->len_tokens)
	{
		if (handle_redirections(cmd->tokens, &k, cmd->len_tokens) != 0)
			return (2);
		else
			k++;
	}
	return (0);
}
