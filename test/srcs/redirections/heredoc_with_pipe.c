/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_with_pipe.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sandriam <sandriam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 11:26:30 by tambinin          #+#    #+#             */
/*   Updated: 2024/12/26 14:23:05 by sandriam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	parse_exec_heredoc_pipe(t_token **tp_arg, int len)
{
	if (contains_heredoc_pipe(tp_arg, len))
	{
		tp_arg[0]->count_heredoc = count_heredoc_pipe(tp_arg,
				tp_arg[0]->count_heredoc, len);
		execute_pipe_heredoc(tp_arg, tp_arg[0]->pipe_heredoc, len,
			tp_arg[0]->count_heredoc);
	}
}

int	contains_heredoc_pipe(t_token **token, int len)
{
	int	i;

	i = 0;
	while (i < len)
	{
		if (token[i]->type == REDIR_HEREDOC)
			return (1);
		i++;
	}
	return (0);
}

int	count_heredoc_pipe(t_token **token, int count_heredoc, int len)
{
	int	i;

	i = 0;
	while (i < len)
	{
		if (token[i]->type == REDIR_HEREDOC)
			count_heredoc++;
		i++;
	}
	return (count_heredoc);
}

void	looop(char *delimiter, int *fd, int is_last_stdin, t_token **token)
{
	char	*input;

	input = NULL;
	set_sig_heredoc(0);
	while (1)
	{
		signal(SIGINT, sig_handler_heredoc_pipe);
		input = readline("heredoc>");
		if (check_signal_exit_hd(input, fd, is_last_stdin, delimiter) == 1)
			break ;
		input = write_fd_input_rd(token, input, is_last_stdin, fd);
		free(input);
	}
}

void	execute_pipe_heredoc(t_token **token, int *pipe_heredoc, int len,
		int len_heredoc)
{
	int		i_hd;
	int		i;
	char	*delimiter;
	int		lst;

	i_hd = 0;
	i = 0;
	lst = 0;
	while (i < len)
	{
		if (token[i]->type == REDIR_HEREDOC && token[i + 1]->type == DELIMITER)
		{
			lst = checks_if_stdin(i, token, len);
			if (i_hd + 1 == len_heredoc && lst == 0 && token[0]->tp_val)
				pipe(pipe_heredoc);
			delimiter = token[i + 1]->value;
			token[0]->tmp_value = token[i + 1]->type_delim;
			looop(delimiter, pipe_heredoc, lst, token);
			if (i_hd + 1 == len_heredoc && lst == 0 && token[0]->tp_val)
				token[0]->fd_heredoc = pipe_heredoc[0];
			i_hd++;
		}
		i++;
	}
}
