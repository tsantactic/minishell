/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe_util_fourth.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sandriam <sandriam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 16:47:06 by sandriam          #+#    #+#             */
/*   Updated: 2024/12/26 14:23:06 by sandriam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	exit_proccess_pipe(t_cmd *cmd)
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
	set_sig_heredoc(0);
}

void	init_execute_heredoc(t_cmd *cmd, t_env **env)
{
	cmd->fd_pipe = NULL;
	check_stdin(cmd);
	check_stdout(cmd);
	init_value_command(cmd, env);
	execute_heredoc_pipe(cmd);
}

void	free_fd_pipe(int i, t_cmd *cmd)
{
	while (i-- > 0)
		free(cmd->fd_pipe[i]);
	free(cmd->fd_pipe);
	free(cmd->pid);
}

void	init_fd_pipes(t_cmd *cmd)
{
	int	i;

	i = 0;
	while (i < cmd->nb_pipe)
	{
		cmd->fd_pipe[i] = malloc(sizeof(int) * 2);
		if (!cmd->fd_pipe[i])
		{
			perror("malloc");
			free_fd_pipe(i, cmd);
			exit(EXIT_FAILURE);
		}
		if (pipe(cmd->fd_pipe[i]) == -1)
		{
			perror("pipe");
			free_fd_pipe(i, cmd);
			exit(EXIT_FAILURE);
		}
		i++;
	}
}

void	init_pipes_and_pids(t_cmd *cmd)
{
	cmd->fd_pipe = malloc(sizeof(int *) * cmd->nb_pipe);
	if (!cmd->fd_pipe)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	cmd->pid = malloc(sizeof(pid_t) * (cmd->nb_pipe + 1));
	if (!cmd->pid)
	{
		perror("malloc");
		free(cmd->fd_pipe);
		exit(EXIT_FAILURE);
	}
	init_fd_pipes(cmd);
}
