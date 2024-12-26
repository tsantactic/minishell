/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_piped_commands.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sandriam <sandriam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 15:27:33 by sandriam          #+#    #+#             */
/*   Updated: 2024/12/26 14:23:02 by sandriam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	handle_pipe(t_cmd *cmd, t_env **env)
{
	int	i;

	init_execute_heredoc(cmd, env);
	if (set_sig_heredoc(-1) == 1)
	{
		if (set_st(-1) == 130)
			exit_proccess_pipe(cmd);
	}
	else
	{
		signal(SIGINT, sig_handler);
		init_pipes_and_pids(cmd);
		i = 0;
		while (i <= cmd->nb_pipe)
		{
			cmd->len_cmd = cmd->tp_arg[i][0]->len_cmd_arg;
			process_execute_cmd(i, cmd, env);
			i++;
		}
		close_fd_pipe(cmd);
		close_fd_heredoc_possible(cmd);
		capture_status_command(cmd);
		free_fd_pipe_pid(cmd);
	}
}
