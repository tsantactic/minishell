/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_not_bin_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sandriam <sandriam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 10:17:17 by sandriam          #+#    #+#             */
/*   Updated: 2024/12/24 11:13:40 by sandriam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	handle_stat_not_path(char **tmp_cmd, char *command, t_env **env)
{
	int	i;
	int	is_not_cmd;
	int	is_not_dir;
	int	flags[2];

	i = 0;
	is_not_cmd = 0;
	is_not_dir = 0;
	while (tmp_cmd[i])
	{
		if (ft_strchr(tmp_cmd[i], '.'))
		{
			is_not_cmd++;
			if (ft_strcmp(tmp_cmd[i], "..") == 0)
				is_not_cmd++;
		}
		if (ft_strchr(tmp_cmd[i], '/'))
			is_not_dir++;
		i++;
	}
	flags[0] = is_not_cmd;
	flags[1] = is_not_dir;
	check_set_status_error(tmp_cmd, flags, command, env);
}

void	execute_if_executable(t_env **env, char **tmp_cmd, char *join_path,
		char *command)
{
	struct stat	path_stat;
	char		**env_list;

	env_list = env_list_to_array(*env);
	if (execve(join_path, tmp_cmd, env_list) == -1)
	{
		ft_putstr_fd("minishell: '", 2);
		ft_putstr_fd(command, 2);
		if (stat(tmp_cmd[0], &path_stat) == 0)
		{
			if (S_ISDIR(path_stat.st_mode))
				ft_putstr_fd("': Is a directory\n", 2);
			else if (S_ISREG(path_stat.st_mode))
				perror("'");
		}
		ft_free(env_list);
		set_st(126);
	}
}

void	execute_cmd_clear(t_env **env)
{
	char	*cmd[2];
	char	**env_list;

	cmd[0] = "/bin/clear";
	cmd[1] = NULL;
	env_list = env_list_to_array(*env);
	if (execve("/bin/clear", cmd, env_list) == -1)
	{
		ft_free(cmd);
		ft_free(env_list);
		ft_putstr_fd("': Is a directory\n", 2);
		set_st(126);
	}
}

void	execute_clear_term(t_env **env, char **tmp_cmd)
{
	if (check_env(env, "TERM=") == 1)
		execute_cmd_clear(env);
	else
	{
		if (execve("/bin/clear", tmp_cmd, NULL) == -1)
		{
			perror("execve failed");
			set_st(1);
		}
	}
}
