/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_simple_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sandriam <sandriam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 09:16:48 by sandriam          #+#    #+#             */
/*   Updated: 2024/12/24 14:31:53 by sandriam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	check_if_stdin(int i, t_cmd *cmd)
{
	int	k;
	int	is_last_stdin;

	k = i + 1;
	is_last_stdin = 0;
	while (k < cmd->len_tokens)
	{
		if (cmd->tokens[k]->type == REDIR_HEREDOC
			|| cmd->tokens[k]->type == REDIR_IN)
			is_last_stdin = 1;
		k++;
	}
	return (is_last_stdin);
}

void	manage_fd(int i, t_cmd *cmd, int **pipe_heredoc, int heredoc_index)
{
	int	is_last_stdin;

	is_last_stdin = check_if_stdin(i, cmd);
	if (heredoc_index + 1 == cmd->count_heredoc && is_last_stdin == 0)
	{
		if (set_st(-1) != 130)
		{
			dup2(pipe_heredoc[heredoc_index][0], STDIN_FILENO);
			close(pipe_heredoc[heredoc_index][1]);
		}
		close(pipe_heredoc[heredoc_index][0]);
	}
	else
		close(pipe_heredoc[heredoc_index][0]);
}

void	execute_heredoc_loop(t_cmd *cmd, int **pipe_heredoc, int heredoc_index,
		int i)
{
	char	*delimiter;

	delimiter = NULL;
	delimiter = cmd->tokens[i + 1]->value;
	pipe(pipe_heredoc[heredoc_index]);
	cmd->type_del = cmd->tokens[i + 1]->type_delim;
	loop_heredoc(delimiter, pipe_heredoc[heredoc_index], cmd, cmd->type_del);
}
