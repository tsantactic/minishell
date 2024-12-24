/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_exit.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tambinin <tambinin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 10:19:58 by tambinin          #+#    #+#             */
/*   Updated: 2024/12/21 11:34:31 by tambinin         ###   ########.fr       */
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
		ft_free_token_cmd(cmd);
		free_tokens(cmd);
		free_commands(cmd);
		free(cmd->commands_arg);
		free_new_env(env);
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

void	free_exit_simple(char **args, t_cmd *cmd, t_env **env, long exit_code)
{
	exit_code = ft_atol(args[1]);
	set_st(exit_code);
	dup2(cmd->stdout_backup, STDOUT_FILENO);
	dup2(cmd->stdin_backup, STDIN_FILENO);
	close(cmd->stdout_backup);
	close(cmd->stdin_backup);
	if (args)
		ft_free(args);
	if (contains_heredoc(cmd))
		free_pipe_heredoc(cmd->count_heredoc, cmd->pipe_heredoc);
	ft_free_token_cmd(cmd);
	free_new_env(env);
	free_tokens(cmd);
	exit(exit_code);
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
	{
		free_exit_simple(args, cmd, env, exit_code);
	}
}

void	ft_exit_simple(t_cmd *cmd, char **args, t_env **env)
{
	long	exit_code;

	exit_code = EXIT_FAILURE;
	if (args[1] == NULL)
	{
		ft_putendl_fd("exit", 1);
		rl_clear_history();
		exit(0);
	}
	else
	{
		exit_code = ft_atol(args[1]);
		exit_simple_utils(cmd, args, env, exit_code);
	}
	set_st(-1);
	rl_clear_history();
	return ;
}
