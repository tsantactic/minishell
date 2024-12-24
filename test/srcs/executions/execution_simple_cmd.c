/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_simple_cmd.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tambinin <tambinin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 10:26:02 by sandriam          #+#    #+#             */
/*   Updated: 2024/12/21 15:20:41 by tambinin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_builtin(char *command)
{
	if (ft_strcmp(command, "cd") == 0)
		return (1);
	if (ft_strcmp(command, "pwd") == 0 || ft_strcmp(command, "/bin/pwd") == 0
		|| ft_strcmp(command, "/usr/bin/pwd") == 0)
		return (2);
	if (ft_strcmp(command, "export") == 0)
		return (3);
	if (ft_strcmp(command, "unset") == 0)
		return (4);
	if (ft_strcmp(command, "env") == 0 || ft_strcmp(command, "/bin/env") == 0
		|| ft_strcmp(command, "/usr/bin/env") == 0)
		return (5);
	if (ft_strcmp(command, "exit") == 0)
		return (6);
	if (ft_strcmp(command, "echo") == 0 || ft_strcmp(command, "/bin/echo") == 0
		|| ft_strcmp(command, "/usr/bin/echo") == 0)
		return (7);
	return (0);
}

char	*set_command(t_cmd *cmd, char *command)
{
	int	i;

	i = 0;
	while (i < cmd->len_tokens)
	{
		if (cmd->tokens[i]->type == 1)
			command = cmd->tokens[i]->value;
		i++;
	}
	return (command);
}

void	fork_without_pipe(t_cmd *cmd, t_env **env)
{
	pid_t	pid;
	int		stat;

	pid = fork();
	if (pid == 0)
	{
		parsing_argument_simple(cmd, env);
		ft_free_token_cmd(cmd);
		free_new_env(env);
		exit(set_st(-1));
	}
	else
	{
		set_st(-1);
		waitpid(pid, &stat, 0);
	}
	if (WIFEXITED(stat))
		set_st(WEXITSTATUS(stat));
	else if (WIFSIGNALED(stat))
	{
		set_st(WTERMSIG(stat) + 128);
		if (set_st(WTERMSIG(stat) + 128) == 131)
			printf("quit (core dumped)\n");
	}
}

void	execute_without_pipe(t_cmd *cmd, t_env **env)
{
	char	*command;

	command = NULL;
	command = set_command(cmd, command);
	if (command && is_builtin(command))
		parsing_arg_with_builtins(cmd, env);
	else
		fork_without_pipe(cmd, env);
	return ;
}
