/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd_simple.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tambinin <tambinin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 11:25:57 by sandriam          #+#    #+#             */
/*   Updated: 2024/12/20 13:59:43 by tambinin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_execute_command(t_cmd *cmd, char *path, char **my_t_cmd,
		t_env **envp)
{
	char	**env;

	env = env_list_to_array(*envp);
	execute_and_handle_error(cmd, path, my_t_cmd, env);
}

void	execute_and_handle_error(t_cmd *cmd, char *path, char **my_t_cmd,
		char **env)
{
	struct stat	path_stat;

	if (execve(path, my_t_cmd, env) == -1)
	{
		if (stat(my_t_cmd[0], &path_stat) == 0)
			handle_stat_error(my_t_cmd, cmd, env);
		else
			handle_errno_case(my_t_cmd, cmd);
		ft_free(env);
	}
	else
	{
		set_st(0);
		cleanup_resources(cmd, env);
	}
}

void	handle_stat_error(char **my_t_cmd, t_cmd *cmd, char **env)
{
	int	i;
	int	is_not_cmd;
	int	is_not_dir;

	i = 0;
	is_not_cmd = 0;
	is_not_dir = 0;
	while (i < cmd->len_cmd)
	{
		if (ft_strchr(my_t_cmd[i], '.'))
		{
			is_not_cmd++;
			if (ft_strcmp(my_t_cmd[i], "..") == 0)
				is_not_cmd++;
		}
		if (ft_strchr(my_t_cmd[i], '/'))
			is_not_dir++;
		i++;
	}
	check_and_set_exit_status(my_t_cmd, is_not_cmd, is_not_dir, env);
}

int	errno_loop(char **my_t_cmd, t_cmd *cmd)
{
	int	is_not_cmd;
	int	i;

	i = 0;
	is_not_cmd = 0;
	while (i < cmd->len_cmd)
	{
		if (ft_strchr(my_t_cmd[i], '/'))
		{
			is_not_cmd = 1;
			break ;
		}
		i++;
	}
	return (is_not_cmd);
}

void	handle_errno_case(char **my_t_cmd, t_cmd *cmd)
{
	int	is_not_cmd;

	ft_putstr_fd("minishell: '", 2);
	ft_putstr_fd(my_t_cmd[0], 2);
	if (errno == 20)
	{
		ft_putstr_fd("': Not a directory\n", 2);
		set_st(126);
	}
	else if (errno == 2)
	{
		is_not_cmd = errno_loop(my_t_cmd, cmd);
		if (is_not_cmd)
		{
			ft_putstr_fd("': No such file or directory\n", 2);
			set_st(127);
		}
		else
		{
			ft_putstr_fd("': command not found\n", 2);
			set_st(127);
		}
	}
}
