/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_not_bin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sandriam <sandriam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 09:46:27 by sandriam          #+#    #+#             */
/*   Updated: 2024/12/24 10:42:15 by sandriam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	display_and_exec(t_env **env, char **tmp_cmd, char *command)
{
	char	cwd[1024];
	char	*join_path;

	join_path = ft_strcat(getcwd(cwd, sizeof(cwd)), "/");
	join_path = ft_strcat(join_path, command);
	if (ft_strcmp(join_path, "/usr/bin/clear") == 0 || ft_strcmp(join_path,
			"/bin/clear") == 0)
		execute_clear_term(env, tmp_cmd);
	else
		execute_if_executable(env, tmp_cmd, join_path, command);
}

void	check_set_status_error(char **tmp_cmd, int *flags, char *command,
		t_env **env)
{
	if (flags[0] == 1 && !flags[1] && tmp_cmd[0][0] == '.')
	{
		ft_putstr_fd("minishell: '", 2);
		ft_putstr_fd(tmp_cmd[0], 2);
		ft_putstr_fd("': filename required\n.:usage: . file[arg]\n",
			2);
		set_st(2);
	}
	else if (flags[0] == 2 && !flags[1])
	{
		ft_putstr_fd("minishell: '", 2);
		ft_putstr_fd(command, 2);
		ft_putstr_fd("': Is a directory\n", 2);
		set_st(126);
	}
	else
		display_and_exec(env, tmp_cmd, command);
}

void	path_not_set_not_bin(char *command, t_env **env, char **tmp_cmd)
{
	struct stat	path_stat;

	if (stat(tmp_cmd[0], &path_stat) == 0)
		handle_stat_not_path(tmp_cmd, command, env);
	else
	{
		ft_putstr_fd("minishell: '", 2);
		ft_putstr_fd(tmp_cmd[0], 2);
		if (errno == 20)
		{
			ft_putstr_fd("': Not a directory\n", 2);
			set_st(126);
		}
		else if (errno == 2)
		{
			ft_putstr_fd("': No such file or directory\n", 2);
			set_st(127);
		}
	}
}
