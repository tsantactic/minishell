/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_part_two.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tambinin <tambinin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 10:40:38 by tambinin          #+#    #+#             */
/*   Updated: 2024/12/23 10:41:43 by tambinin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	sig_handler_heredoc(int signal)
{
	if (signal == SIGINT)
	{
		set_st(130);
		set_sig_heredoc(1);
		close(STDIN_FILENO);
	}
}

void	sig_handler_heredoc_blt(int signal)
{
	if (signal == SIGINT)
	{
		write(1, "\n", 1);
		set_st(130);
		set_sig_heredoc(1);
		close(STDIN_FILENO);
	}
}