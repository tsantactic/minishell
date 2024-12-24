/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_cd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tambinin <tambinin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 10:20:23 by tambinin          #+#    #+#             */
/*   Updated: 2024/12/21 13:27:18 by tambinin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	build_cd(char **args, t_env **env)
{
	const char	*home;

	if (args[1] && args[2])
	{
		ft_putendl_fd(" too many arguments", STDERR_FILENO);
		set_st(1);
		return ;
	}
	if (!args[1])
	{
		home = get_env_value("HOME", env);
		if (!home || chdir(home) != 0)
		{
			set_st(1);
			perror("cd");
			return ;
		}
	}
	else if (chdir(args[1]) != 0)
	{
		set_st(1);
		perror("cd");
		return ;
	}
}

void	ft_cd(char **args, t_env **env)
{
	const char	*old_pwd;
	char		cwd[1024];

	old_pwd = get_env_value("PWD", env);
	set_st(0);
	build_cd(args, env);
	if (old_pwd)
		update_env_var("OLDPWD", old_pwd, env);
	if (getcwd(cwd, sizeof(cwd)) != NULL)
		update_env_var("PWD", cwd, env);
	else
		perror("getcwd");
}
