/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executions_loop.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tambinin <tambinin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 10:59:59 by tambinin          #+#    #+#             */
/*   Updated: 2024/12/22 17:40:12 by tambinin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	handle_input_heredoc(char **input, char *delimiter)
{
	*input = readline("heredoc>");
	handle_sigint(*input);
	if (!*input && set_sig_heredoc(-1) != 1)
	{
		set_sig_new_line(1);
		ft_putstr_fd("minishel : warning: here-document"
			"at this line delimited by end-of-file (wanted :'", 2);
		ft_putstr_fd(delimiter, 2);
		ft_putstr_fd("')\n", 2);
		set_st(-1);
	}
}

int	is_delimiter_reached(char *input, char *delimiter)
{
	if (ft_strcmp(input, delimiter) == 0)
	{
		free(input);
		return (1);
	}
	return (0);
}

void	handle_sigint(char *input)
{
	if (set_sig_heredoc(-1) == 1)
	{
		if (input)
			free(input);
	}
}

void	loop_simple_heredoc(char *delimiter, t_cmd *cmd)
{
	char	*input;

	input = NULL;
	set_sig_heredoc(0);
	cmd->stdin_heredoc = dup(STDIN_FILENO);
	while (1)
	{
		signal(SIGINT, sig_handler_heredoc_blt);
		handle_input_heredoc(&input, delimiter);
		if (!input || set_sig_heredoc(-1) == 1)
			break ;
		if (is_delimiter_reached(input, delimiter))
			break ;
		free(input);
	}
	dup2(cmd->stdin_heredoc, 0);
	close(cmd->stdin_heredoc);
}
