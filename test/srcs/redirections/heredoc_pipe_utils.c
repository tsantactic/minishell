/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_pipe_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sandriam <sandriam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 09:16:45 by sandriam          #+#    #+#             */
/*   Updated: 2024/12/26 14:23:04 by sandriam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	checks_if_stdin(int i, t_token **token, int len)
{
	int	is_last_stdin;
	int	k;

	is_last_stdin = 0;
	k = i + 1;
	while (k < len)
	{
		if (token[k]->type == REDIR_HEREDOC || token[k]->type == REDIR_IN)
			is_last_stdin = 1;
		k++;
	}
	if (is_last_stdin == 0 && !token[0]->tp_val)
		is_last_stdin = 1;
	return (is_last_stdin);
}

char	*write_fd_input_rd(t_token **token, char *input, int is_last_stdin,
		int *fd)
{
	char	*expanded;

	if (is_last_stdin == 0)
	{
		if (token[0]->tmp_value == DELIMITER_NO_QUOTE)
		{
			expanded = expand_heredoc_pipe(input, token[0]->env_lst);
			free(input);
			input = expanded;
		}
		ft_putendl_fd(input, fd[1]);
	}
	return (input);
}

void	ft_print_warning_signal(char *delimiter, int *fd, int is_last_stdin)
{
	ft_putstr_fd("minishel : warning: (wanted :'",
		2);
	ft_putstr_fd(delimiter, 2);
	ft_putstr_fd("')\n", 2);
	if (is_last_stdin == 0)
		close(fd[1]);
	set_st(0);
}

int	check_signal_exit_hd(char *input, int *fd, int is_last_stdin,
		char *delimiter)
{
	if (set_sig_heredoc(-1) == 1)
	{
		if (input)
			free(input);
		if (is_last_stdin == 0)
			close(fd[1]);
		open("/dev/tty", O_RDONLY);
		return (1);
	}
	if (!input)
	{
		ft_print_warning_signal(delimiter, fd, is_last_stdin);
		return (1);
	}
	if (ft_strcmp(input, delimiter) == 0)
	{
		free(input);
		if (is_last_stdin == 0)
			close(fd[1]);
		set_st(0);
		return (1);
	}
	return (0);
}
