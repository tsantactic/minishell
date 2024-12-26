/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_exit.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sandriam <sandriam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 10:19:58 by tambinin          #+#    #+#             */
/*   Updated: 2024/12/24 13:59:29 by sandriam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	exit_pipe_utils(char **args, t_cmd *cmd, t_env **env, long exit_code)
{
	if (exit_code == -13464121485485)
	{
		ft_putendl_fd("exit: numeric argument required", 2);
		set_st(2);
		rl_clear_history();
	}
	else if (args[2] != NULL)
	{
		set_st(1);
		ft_putendl_fd("exit: too many arguments", 2);
		return ;
	}
	else
	{
		exit_code = ft_atol(args[1]);
		set_st(exit_code);
		free_exit_pipe(cmd, env);
		exit(exit_code);
	}
}

void	ft_exit_pipe(char **args, t_cmd *cmd, t_env **env, char *extract_cmd)
{
	long	exit_code;

	exit_code = EXIT_FAILURE;
	if (args[1] == NULL)
	{
		ft_putendl_fd("exit", 1);
		rl_clear_history();
		free(extract_cmd);
		free_exit_pipe(cmd, env);
		exit(0);
	}
	else
	{
		exit_code = ft_atol(args[1]);
		free(extract_cmd);
		exit_pipe_utils(args, cmd, env, exit_code);
	}
	set_st(-1);
	rl_clear_history();
	return ;
}

void	exit_simple_utils(t_cmd *cmd, char **args, t_env **env, long exit_code)
{
	if (exit_code == -13464121485485)
	{
		ft_putendl_fd("exit: numeric argument required", 2);
		set_st(2);
		rl_clear_history();
	}
	else if (args[2] != NULL)
	{
		set_st(1);
		ft_putendl_fd("exit: too many arguments", 2);
		return ;
	}
	else
		free_exit_simple(args, cmd, env, exit_code);
}

void	ft_exit_simple(t_cmd *cmd, char **args, t_env **env)
{
	long	exit_code;

	exit_code = EXIT_FAILURE;
	if (args[1] == NULL)
	{
		ft_putendl_fd("exit", 1);
		rl_clear_history();
		free_exit_no_arg(cmd, args, env);
		exit(0);
	}
	else
	{
		exit_code = ft_atol(args[1]);
		exit_simple_utils(cmd, args, env, exit_code);
	}
	set_st(-1);
	return ;
}
