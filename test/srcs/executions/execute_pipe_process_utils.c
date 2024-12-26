/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe_process_utils.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sandriam <sandriam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 17:37:07 by sandriam          #+#    #+#             */
/*   Updated: 2024/12/25 11:36:48 by sandriam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	free_exit_processus_pipe(t_cmd *cmd, t_env **env)
{
	close_fd_heredoc_possible(cmd);
	ft_free_token_cmd(cmd);
	free_new_env(env);
	free_commands(cmd);
	free_tokens(cmd);
	free(cmd->tp_arg);
	free_fd_pipe_pid(cmd);
	exit(set_st(-1));
}

void	init_stdin_pipe(int i, t_cmd *cmd)
{
	if (i > 0)
	{
		if (cmd->tp_arg[i][0]->stdin_set == 1)
		{
			if (dup2(cmd->fd_pipe[i - 1][0], STDIN_FILENO) == -1)
			{
				perror("dup2");
				exit(EXIT_FAILURE);
			}
		}
		else
			close(cmd->fd_pipe[i - 1][0]);
	}
	if (i < cmd->nb_pipe)
	{
		if (dup2(cmd->fd_pipe[i][1], STDOUT_FILENO) == -1)
		{
			perror("dup2");
			exit(EXIT_FAILURE);
		}
	}
}

void	init_stdin_heredoc_fd(int i, t_cmd *cmd)
{
	if (contains_heredoc_pipe(cmd->tp_arg[i], cmd->tp_arg[i][0]->total_len)
		&& cmd->tp_arg[i][0]->fd_heredoc != -1)
	{
		if (cmd->tp_arg[i][0]->fd_heredoc)
		{
			dup2(cmd->tp_arg[i][0]->fd_heredoc, STDIN_FILENO);
			close(cmd->tp_arg[i][0]->fd_heredoc);
		}
	}
}

void	exit_process_by_redir(t_cmd *cmd, t_env **env)
{
	close_fd_heredoc_possible(cmd);
	close_fd_pipe(cmd);
	free_fd_pipe_pid(cmd);
	ft_free_token_cmd(cmd);
	free_tokens(cmd);
	free_new_env(env);
	free_commands(cmd);
	free(cmd->tp_arg);
	exit(1);
}

void	exit_by_not_found(t_cmd *cmd, t_env **env)
{
	close_fd_heredoc_possible(cmd);
	ft_free_token_cmd(cmd);
	free_new_env(env);
	free_commands(cmd);
	free_tokens(cmd);
	free(cmd->tp_arg);
	free_fd_pipe_pid(cmd);
}
