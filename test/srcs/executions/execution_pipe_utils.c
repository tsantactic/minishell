/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_pipe_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sandriam <sandriam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 14:23:23 by sandriam          #+#    #+#             */
/*   Updated: 2024/12/26 14:23:05 by sandriam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	free_commands(t_cmd *cmd)
{
	int	i;
	int	j;

	i = 0;
	while (i <= cmd->nb_pipe)
	{
		if (cmd->tp_arg[i][0]->tp_val)
			free(cmd->tp_arg[i][0]->tp_val);
		ft_free(cmd->tp_arg[i][0]->tp_args);
		j = 0;
		while (cmd->tp_arg[i][j])
		{
			free(cmd->tp_arg[i][j]);
			j++;
		}
		free(cmd->tp_arg[i]);
		i++;
	}
}

void	init_value_command(t_cmd *cmd, t_env **env)
{
	int	i;

	i = 0;
	while (i <= cmd->nb_pipe)
	{
		cmd->tp_arg[i][0]->env_lst = env;
		cmd->tp_arg[i][0]->count_heredoc = 0;
		cmd->tp_arg[i][0]->fd_heredoc = -1;
		i++;
	}
}

void	execute_heredoc_pipe(t_cmd *cmd)
{
	int	i;

	i = 0;
	while (i <= cmd->nb_pipe)
	{
		parse_exec_heredoc_pipe(cmd->tp_arg[i],
			cmd->tp_arg[i][0]->total_len);
		if (set_st(-1) == 130 && set_sig_heredoc(-1) == 1)
			break ;
		i++;
	}
}
