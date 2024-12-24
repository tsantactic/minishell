/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_not_set.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sandriam <sandriam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 09:46:30 by sandriam          #+#    #+#             */
/*   Updated: 2024/12/24 11:11:14 by sandriam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	execute_clears_cmd(void)
{
	char	*cmd[2];
	char	*env_term[2];

	env_term[0] = "TERM=xterm";
	env_term[1] = NULL;
	cmd[0] = "/bin/clear";
	cmd[1] = NULL;
	if (execve("/bin/clear", cmd, env_term) == -1)
	{
		ft_free(cmd);
		ft_free(env_term);
		perror("execve failed");
		exit(1);
	}
	ft_free(env_term);
}

void	execute_cmd_executable(t_env **env, char *command, char **tmp_cmd)
{
	char	**env_list;

	env_list = env_list_to_array(*env);
	if (execve(command, tmp_cmd, env_list) == -1)
	{
		path_not_set_not_bin(command, env, tmp_cmd);
		ft_free(env_list);
	}
	else
		set_st(0);
}

void	path_not_set_bin(char *command, t_env **env, char **tmp_cmd)
{
	if (ft_strcmp(command, "/usr/bin/clear") == 0 || ft_strcmp(command,
			"/bin/clear") == 0)
	{
		if (check_env(env, "TERM=") == 1)
			execute_clears_cmd();
		else
		{
			if (execve("/bin/clear", tmp_cmd, NULL) == -1)
			{
				perror("execve failed");
				exit(1);
			}
		}
	}
	else
		execute_cmd_executable(env, command, tmp_cmd);
}
