/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command_pipe_utils.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sandriam <sandriam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 17:58:47 by sandriam          #+#    #+#             */
/*   Updated: 2024/12/25 12:35:28 by sandriam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	execute_command_all(t_cmd *cmd, t_env **env, int i)
{
	char	*path;

	path = NULL;
	if (contains_bin(cmd->tp_arg[i][0]->tp_val))
		cmd->extracted_command = extract_command_bin(cmd->tp_arg[i][0]->tp_val);
	else
		cmd->extracted_command = cmd->tp_arg[i][0]->tp_val;
	if (!ft_strchr(cmd->extracted_command, '.')
		&& !ft_strchr(cmd->extracted_command, '/'))
		path = ft_find_path(cmd->extracted_command, env);
	if (ft_strchr(cmd->tp_arg[i][0]->tp_val, '/') == 0
		|| ft_strchr(cmd->extracted_command, '/') == 0)
	{
		cmd->tmp_cmd = cmd->tp_arg[i][0]->tp_args;
		path_is_here(path, env, cmd->tp_arg[i][0]->tp_val, cmd);
	}
	else
	{
		execute_with_stat(env, cmd, cmd->tp_arg[i][0]->tp_args);
		if (path)
			free(path);
	}
}

void	check_and_execute(t_cmd *cmd, int i, t_env **env)
{
	if (check_env(env, "PATH=") == 1)
		execute_command_all(cmd, env, i);
	else
		path_not_set(cmd->tp_arg[i][0]->tp_val, env,
			cmd->tp_arg[i][0]->tp_args);
}

void	execute_command_exist(t_cmd *cmd, t_env **env, int i)
{
	char	*extract_command;

	if (contains_bin(cmd->tp_arg[i][0]->tp_val))
		extract_command = extract_command_bin(cmd->tp_arg[i][0]->tp_val);
	else
		extract_command = cmd->tp_arg[i][0]->tp_val;
	if (is_builtin(extract_command))
	{
		close_fd_heredoc_possible(cmd);
		execute_builtins_pipe(i, extract_command, env, cmd);
		exit(set_st(-1));
	}
	else
	{
		check_and_execute(cmd, i, env);
		close_fd_pipe(cmd);
		close_fd_heredoc_possible(cmd);
		ft_free_token_cmd(cmd);
		free_new_env(env);
		free_commands(cmd);
		free_tokens(cmd);
		free(cmd->tp_arg);
		free_fd_pipe_pid(cmd);
		exit(set_st(-1));
	}
}
