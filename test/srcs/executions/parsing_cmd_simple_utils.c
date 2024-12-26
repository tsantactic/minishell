/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_cmd_simple_utils.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sandriam <sandriam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 09:42:46 by sandriam          #+#    #+#             */
/*   Updated: 2024/12/25 11:03:45 by sandriam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	cleanup_parsing(t_cmd *cmd, char **tmp_cmd)
{
	if (contains_heredoc(cmd))
		free_pipe_heredoc(cmd->count_heredoc, cmd->pipe_heredoc);
	free_tokens(cmd);
	if (tmp_cmd)
		ft_free(tmp_cmd);
}

void	execute_with_stat(t_env **env, t_cmd *cmd, char **tmp_cmd)
{
	struct stat	path_stat;
	char		**envp;

	envp = env_list_to_array(*env);
	if (stat(tmp_cmd[0], &path_stat) == 0)
		handle_stat_error(tmp_cmd, cmd, envp);
	else
		handle_errno_case(tmp_cmd, cmd);
	ft_free(envp);
}

void	execute_cmd_with_path(char *command, t_cmd *cmd, char **tmp_cmd,
		t_env **env)
{
	char	*path;

	path = NULL;
	if (contains_bin(command))
		cmd->extracted_command = extract_command_bin(command);
	else
		cmd->extracted_command = command;
	if (!ft_strchr(cmd->extracted_command, '.')
		&& !ft_strchr(cmd->extracted_command, '/'))
		path = ft_find_path(cmd->extracted_command, env);
	if (ft_strchr(command, '/') == 0 || ft_strchr(cmd->extracted_command,
			'/') == 0)
	{
		cmd->tmp_cmd = tmp_cmd;
		path_is_here(path, env, command, cmd);
	}
	else
	{
		execute_with_stat(env, cmd, tmp_cmd);
		if (path)
			free(path);
	}
}

void	handle_command_execution(t_cmd *cmd, char *command, char **tmp_cmd,
		t_env **env)
{
	if (check_env(env, "PATH=") == 1)
		execute_cmd_with_path(command, cmd, tmp_cmd, env);
	else
		path_not_set(command, env, tmp_cmd);
	free(command);
	if (contains_heredoc(cmd))
		free_pipe_heredoc(cmd->count_heredoc, cmd->pipe_heredoc);
}
