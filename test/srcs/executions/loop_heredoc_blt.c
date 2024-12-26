/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop_heredoc_blt.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sandriam <sandriam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 10:35:22 by tambinin          #+#    #+#             */
/*   Updated: 2024/12/24 12:08:50 by sandriam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	handle_input_signal(char *input, int *pipefd)
{
	set_sig_new_line(1);
	if (input)
		free(input);
	close(pipefd[1]);
}

void	handle_warning_text(char *delimiter, int *pipefd)
{
	set_sig_new_line(1);
	ft_putstr_fd("minishel : warning: herdoc by eof (wanted :'", 2);
	ft_putstr_fd(delimiter, 2);
	ft_putstr_fd("')\n", 2);
	close(pipefd[1]);
	set_st(0);
}

int	process_heredoc_input(char *delimiter, int *pipefd)
{
	char	*input;

	input = readline("heredoc>");
	if (set_sig_heredoc(-1) == 1)
	{
		handle_input_signal(input, pipefd);
		return (1);
	}
	if (!input)
	{
		handle_warning_text(delimiter, pipefd);
		return (1);
	}
	if (ft_strcmp(input, delimiter) == 0)
	{
		free(input);
		close(pipefd[1]);
		set_st(0);
		return (1);
	}
	ft_putendl_fd(input, pipefd[1]);
	free(input);
	return (0);
}

void	loop_heredoc_blt(char *delimiter, int *pipefd)
{
	set_sig_heredoc(0);
	while (1)
	{
		signal(SIGINT, sig_handler_heredoc);
		if (process_heredoc_input(delimiter, pipefd) == 1)
			break ;
	}
}
