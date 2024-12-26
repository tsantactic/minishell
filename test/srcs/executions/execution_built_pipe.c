/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_built_pipe.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sandriam <sandriam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 18:18:18 by sandriam          #+#    #+#             */
/*   Updated: 2024/12/24 18:22:24 by sandriam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	execute_builtins_cmd_pipe(int i, char *extract_command, t_cmd *cmd,
		t_env **env)
{
	if (ft_strcmp(extract_command, "echo") == 0)
		ft_echo(cmd->tp_arg[i][0]->tp_args);
	if (ft_strcmp(extract_command, "pwd") == 0)
		ft_pwd(cmd->tp_arg[i][0]->tp_args);
	if (ft_strcmp(extract_command, "env") == 0)
		ft_env(cmd->tp_arg[i][0]->tp_args, env);
	if (ft_strcmp(extract_command, "export") == 0)
		ft_export(cmd->tp_arg[i][0]->tp_args, env);
	if (ft_strcmp(extract_command, "unset") == 0)
		ft_unset(cmd->tp_arg[i][0]->tp_args, env);
	if (ft_strcmp(extract_command, "cd") == 0)
		ft_cd(cmd->tp_arg[i][0]->tp_args, env);
	if (ft_strcmp(extract_command, "exit") == 0)
		ft_exit_pipe(cmd->tp_arg[i][0]->tp_args, cmd, env, extract_command);
}

void	execute_builtins_pipe(int i, char *extract_command, t_env **env,
		t_cmd *cmd)
{
	cmd->index_exit = i;
	if (i != cmd->nb_pipe)
	{
		if (cmd->tp_arg[i + 1][0]->stdin_set == 1
			|| cmd->tp_arg[i][0]->stdout_set == 1)
			execute_builtins_cmd_pipe(i, extract_command, cmd, env);
	}
	else
		execute_builtins_cmd_pipe(i, extract_command, cmd, env);
	ft_free_token_cmd(cmd);
	free_tokens(cmd);
	free_commands(cmd);
	free(cmd->tp_arg);
	free_new_env(env);
	free_fd_pipe_pid(cmd);
}
