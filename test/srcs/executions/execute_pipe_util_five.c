/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe_util_five.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sandriam <sandriam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 16:53:06 by sandriam          #+#    #+#             */
/*   Updated: 2024/12/24 18:02:27 by sandriam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	close_fd_pipe(t_cmd *cmd)
{
	int	j;

	j = 0;
	while (j < cmd->nb_pipe)
	{
		close(cmd->fd_pipe[j][0]);
		close(cmd->fd_pipe[j][1]);
		j++;
	}
}

void	close_fd_heredoc_possible(t_cmd *cmd)
{
	int	i;

	i = 0;
	while (i <= cmd->nb_pipe)
	{
		if (contains_heredoc_pipe(cmd->tp_arg[i], cmd->tp_arg[i][0]->total_len)
			&& cmd->tp_arg[i][0]->fd_heredoc != -1)
		{
			if (cmd->tp_arg[i][0]->fd_heredoc != -1)
				close(cmd->tp_arg[i][0]->fd_heredoc);
		}
		i++;
	}
}

void	free_fd_pipe_pid(t_cmd *cmd)
{
	int	i;

	i = -1;
	while (++i < cmd->nb_pipe)
	{
		free(cmd->fd_pipe[i]);
	}
	free(cmd->pid);
	free(cmd->fd_pipe);
}

void	capture_status_command(t_cmd *cmd)
{
	int	stat;
	int	i;

	i = 0;
	while (i <= cmd->nb_pipe)
	{
		waitpid(cmd->pid[i], &stat, 0);
		i++;
	}
	if (WIFEXITED(stat))
	{
		set_st(WEXITSTATUS(stat));
	}
	else if (WIFSIGNALED(stat))
	{
		set_st(WTERMSIG(stat) + 128);
		if (set_st(WTERMSIG(stat) + 128) == 131)
			printf("quit (core dumped)\n");
	}
}
