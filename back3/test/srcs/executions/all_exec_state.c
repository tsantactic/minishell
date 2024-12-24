/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   all_exec_state.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sandriam <sandriam@student.42antanana      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 10:21:38 by sandriam          #+#    #+#             */
/*   Updated: 2024/12/23 13:26:49 by sandriam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	path_is_here(char *path, t_env **env, char *command,
		t_cmd *cmd)
{
	struct stat	path_stat;
	char		**envp;

	if (path && !ft_strchr(command, '.') && !ft_strchr(command, '/'))
	{
		signal(SIGQUIT, sig_quit_slash);
		ft_execute_command(cmd, path, cmd->tmp_cmd, env);
		free(path);
	}
	else
	{
		envp = env_list_to_array(*env);
		if (stat(cmd->tmp_cmd[0], &path_stat) == 0)
			handle_stat_error(cmd->tmp_cmd, cmd, envp);
		else
			handle_errno_case(cmd->tmp_cmd, cmd);
		ft_free(envp);
		if (path)
			free(path);
	}
}

void	path_not_set(char *command, t_env **env, char **tmp_cmd)
{
	if (contains_bin(command))
	{
		path_not_set_bin(command, env, tmp_cmd);
	}
	else
	{
		path_not_set_not_bin(command, env, tmp_cmd);
	}
}
