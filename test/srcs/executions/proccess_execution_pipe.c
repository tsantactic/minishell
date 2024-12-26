/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   proccess_execution_pipe.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sandriam <sandriam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 16:47:11 by sandriam          #+#    #+#             */
/*   Updated: 2024/12/25 11:17:13 by sandriam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	process_execute_cmd(int i, t_cmd *cmd, t_env **env)
{
	cmd->pid[i] = fork();
	if (cmd->pid[i] == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if (cmd->pid[i] == 0)
	{
		init_stdin_pipe(i, cmd);
		init_stdin_heredoc_fd(i, cmd);
		if (parse_exec_redir_pipe(cmd->tp_arg[i],
				cmd->tp_arg[i][0]->total_len) == 1)
			exit_process_by_redir(cmd, env);
		close_fd_pipe(cmd);
		if (cmd->tp_arg[i][0]->tp_val)
			execute_command_exist(cmd, env, i);
		else
			free_exit_processus_pipe(cmd, env);
	}
}
