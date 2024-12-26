/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_heredoc_blt.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sandriam <sandriam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 10:51:39 by tambinin          #+#    #+#             */
/*   Updated: 2024/12/24 12:13:50 by sandriam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	process_heredoc_step(int **pipe_heredoc, int heredoc_index,
		int *temp_pipe_heredoc, int count_heredoc)
{
	if (heredoc_index + 1 == count_heredoc)
	{
		*temp_pipe_heredoc = pipe_heredoc[heredoc_index][0];
		close(pipe_heredoc[heredoc_index][1]);
		close(pipe_heredoc[heredoc_index][0]);
	}
	else
	{
		close(pipe_heredoc[heredoc_index][0]);
	}
}

void	process_heredoc(char *delimiter, int **pipe_heredoc, int heredoc_index)
{
	pipe(pipe_heredoc[heredoc_index]);
	loop_heredoc_blt(delimiter, pipe_heredoc[heredoc_index]);
	if (set_sig_heredoc(-1) == 1)
	{
		close(pipe_heredoc[heredoc_index][0]);
	}
}

void	execute_heredoc_builtins(t_cmd *cmd, int **pipe_heredoc,
		int count_heredoc, int *temp_pipe_heredoc)
{
	int		heredoc_index;
	int		i;
	char	*delimiter;

	heredoc_index = 0;
	i = 0;
	while (i < cmd->len_tokens)
	{
		if (cmd->tokens[i]->type == REDIR_HEREDOC)
		{
			if (cmd->tokens[i + 1]->type == DELIMITER)
			{
				delimiter = cmd->tokens[i + 1]->value;
				process_heredoc(delimiter, pipe_heredoc, heredoc_index);
				process_heredoc_step(pipe_heredoc, heredoc_index,
					temp_pipe_heredoc, count_heredoc);
			}
			heredoc_index++;
			i += 1;
		}
		if (set_sig_heredoc(-1) == 1)
			break ;
		i++;
	}
}
