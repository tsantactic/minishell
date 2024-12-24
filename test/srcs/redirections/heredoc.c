/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sandriam <sandriam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 10:29:04 by sandriam          #+#    #+#             */
/*   Updated: 2024/12/24 09:16:56 by sandriam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	parse_exec_heredoc(t_cmd *cmd)
{
	int	i;

	cmd->pipe_heredoc = NULL;
	cmd->count_heredoc = 0;
	if (contains_heredoc(cmd))
	{
		cmd->count_heredoc = count_heredoc_arg(cmd, cmd->count_heredoc);
		cmd->pipe_heredoc = malloc(sizeof(int *) * cmd->count_heredoc);
		i = 0;
		while (i < cmd->count_heredoc)
			cmd->pipe_heredoc[i++] = malloc(sizeof(int) * 2);
		execute_heredoc(cmd, cmd->pipe_heredoc);
	}
}

int	contains_heredoc(t_cmd *cmd)
{
	int	i;

	i = 0;
	while (i < cmd->len_arg)
	{
		if (cmd->tokens[i]->type == REDIR_HEREDOC)
			return (1);
		i++;
	}
	return (0);
}

int	count_heredoc_arg(t_cmd *cmd, int count_heredoc)
{
	int	i;

	i = 0;
	while (i < cmd->len_tokens)
	{
		if (cmd->tokens[i]->type == REDIR_HEREDOC)
			count_heredoc++;
		i++;
	}
	return (count_heredoc);
}

void	execute_heredoc(t_cmd *cmd, int **pipe_heredoc)
{
	int	heredoc_index;
	int	i;

	heredoc_index = 0;
	i = 0;
	while (i < cmd->len_tokens)
	{
		cmd->type_del = -1;
		if (cmd->tokens[i]->type == REDIR_HEREDOC)
		{
			if (cmd->tokens[i + 1]->type == DELIMITER)
			{
				execute_heredoc_loop(cmd, pipe_heredoc, heredoc_index, i);
				if (set_sig_heredoc(-1) == 1)
				{
					close(pipe_heredoc[heredoc_index][0]);
					break ;
				}
				manage_fd(i, cmd, pipe_heredoc, heredoc_index);
				heredoc_index++;
			}
			i += 1;
		}
		i++;
	}
}
