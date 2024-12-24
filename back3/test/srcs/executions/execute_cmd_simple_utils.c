/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd_simple_utils.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tambinin <tambinin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 14:59:21 by tambinin          #+#    #+#             */
/*   Updated: 2024/12/21 15:07:37 by tambinin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	execute_if_success(char **my_t_cmd, char **env)
{
	struct stat	path_stat;

	if (execve(my_t_cmd[0], my_t_cmd, env) == -1)
	{
		ft_putstr_fd("minishell: '", 2);
		ft_putstr_fd(my_t_cmd[0], 2);
		if (stat(my_t_cmd[0], &path_stat) == 0)
		{
			if (S_ISDIR(path_stat.st_mode))
				ft_putstr_fd("': Is a directory\n", 2);
			else if (S_ISREG(path_stat.st_mode))
				perror("'");
		}
		set_st(126);
	}
}

void	check_and_set_exit_status(char **my_t_cmd, int is_not_cmd,
		int is_not_dir, char **env)
{
	if (is_not_cmd == 1 && !is_not_dir && my_t_cmd[0][0] == '.')
	{
		ft_putstr_fd("minishell: '", 2);
		ft_putstr_fd(my_t_cmd[0], 2);
		ft_putstr_fd("':filename required\n.: usage: .filename [arguments]\n",
			2);
		set_st(2);
	}
	else if ((is_not_cmd == 2 && !is_not_dir) || ((is_not_cmd == 0
				|| is_not_cmd == 1) && is_not_dir == 0))
	{
		ft_putstr_fd("minishell: '", 2);
		ft_putstr_fd(my_t_cmd[0], 2);
		ft_putstr_fd("': command not found\n", 2);
		set_st(127);
	}
	else
	{
		execute_if_success(my_t_cmd, env);
	}
}

void	cleanup_resources(t_cmd *cmd, char **env)
{
	int	i;

	i = 0;
	while (i < cmd->len_tokens)
	{
		free(cmd->tokens[i]->value);
		free(cmd->tokens[i]);
		i++;
	}
	free(cmd->tokens);
	ft_free(env);
}
