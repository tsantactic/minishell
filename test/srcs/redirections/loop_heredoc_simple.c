/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop_heredoc_simple.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sandriam <sandriam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 09:17:01 by sandriam          #+#    #+#             */
/*   Updated: 2024/12/24 14:31:56 by sandriam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*write_fd_input(int type_quote_delim, char *input, int *pipefd,
		t_cmd *cmd)
{
	char	*expanded;

	if (type_quote_delim == DELIMITER_NO_QUOTE)
	{
		expanded = expand_heredoc_pipe(input, cmd->env_lst);
		free(input);
		input = expanded;
	}
	ft_putendl_fd(input, pipefd[1]);
	return (input);
}

void	ft_print_warning(char *delimiter, int *pipefd)
{
	ft_putstr_fd("minishel : warning: (wanted :'",
		2);
	ft_putstr_fd(delimiter, 2);
	ft_putstr_fd("')\n", 2);
	close(pipefd[1]);
	set_st(0);
}

int	signal_and_exit(char *input, char *delimiter, int *pipefd)
{
	if (set_sig_heredoc(-1) == 1)
	{
		if (input)
			free(input);
		close(pipefd[1]);
		return (1);
	}
	if (!input)
	{
		ft_print_warning(delimiter, pipefd);
		return (1);
	}
	if (ft_strcmp(input, delimiter) == 0)
	{
		free(input);
		close(pipefd[1]);
		set_st(0);
		return (1);
	}
	return (0);
}

void	loop_heredoc(char *delimiter, int *pipefd, t_cmd *cmd,
		int type_quote_delim)
{
	char	*input;

	input = NULL;
	set_sig_heredoc(0);
	cmd->stdin_heredoc = dup(STDIN_FILENO);
	while (1)
	{
		signal(SIGINT, sig_handler_heredoc);
		input = readline("heredoc>");
		if (signal_and_exit(input, delimiter, pipefd) == 1)
			break ;
		input = write_fd_input(type_quote_delim, input, pipefd, cmd);
		free(input);
	}
	dup2(cmd->stdin_heredoc, 0);
	close(cmd->stdin_heredoc);
}
