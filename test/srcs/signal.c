/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tambinin <tambinin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 09:40:01 by sandriam          #+#    #+#             */
/*   Updated: 2024/12/23 10:41:30 by tambinin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	sig_handler(int signum)
{
	if (signum == SIGINT)
	{
		if (set_sig_new_line(-1) == 0)
		{
			write(1, "\n", 1);
			rl_replace_line("", 0);
		}
		else
			rl_replace_line("", 0);
		rl_on_new_line();
		if (errno != 0)
			rl_redisplay();
		set_st(130);
	}
}

void	sig_quit_handler(int signum)
{
	(void)signum;
}

void	sig_quit_slash(int signum)
{
	if (signum == SIGQUIT)
		set_st(131);
}

int	set_sig_heredoc(int nb)
{
	static int	st_heredoc;

	if (nb >= 0)
		st_heredoc = nb;
	return (st_heredoc);
}

int	set_sig_new_line(int nb)
{
	static int	st_new_line;

	if (nb >= 0)
		st_new_line = nb;
	return (st_new_line);
}
