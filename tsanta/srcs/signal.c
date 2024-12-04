/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sandriam <sandriam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 09:40:01 by sandriam          #+#    #+#             */
/*   Updated: 2024/12/04 13:48:17 by sandriam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	sig_handler(int signum)
{
	if (signum == SIGINT)
	{
		if (set_sig_new_line(-1) == 0)
			write(1, "\n", 1);
		else
		{
			rl_replace_line("", 0);
		}
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
	{
		set_st(131);
	}
}
int set_sig_heredoc(int nb)
{
	static int st_heredoc;
	if (nb >= 0)
		st_heredoc = nb;
	return (st_heredoc);
}
int set_sig_new_line(int nb)
{
	static int st_new_line;
	if (nb >= 0)
		st_new_line = nb;
	return (st_new_line);
}
void sig_handler_heredoc(int signal)
{
    if (signal == SIGINT)
    {
        set_st(130);
        set_sig_heredoc(1);
		close(STDIN_FILENO);
    }
}

void sig_handler_heredoc_blt(int signal)
{
    if (signal == SIGINT)
    {
		write(1, "\n", 1);
        set_st(130);
        set_sig_heredoc(1);
		close(STDIN_FILENO);
    }
}