/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_with_pipe.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sandriam <sandriam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 10:16:09 by sandriam          #+#    #+#             */
/*   Updated: 2024/12/24 15:57:48 by sandriam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	add_value_arg(int *len, t_cmd *cmd, int i, int j)
{
	if (cmd->tp_arg[i][j]->type == ARG || cmd->tp_arg[i][j]->type == CMD)
	{
		if (cmd->tp_arg[i][j]->type_env == IS_ENV
			&& ft_strchr(cmd->tp_arg[i][j]->value, ' ') != 0)
			(*len) += count_env(cmd->tp_arg[i][j]->value);
		else
		{
			if (cmd->tp_arg[i][j]->type == CMD)
				cmd->tp_arg[i][0]->tp_val = ft_strdup(
						cmd->tp_arg[i][j]->value);
			(*len)++;
		}
	}
}

void	count_cmd_per_arg(t_cmd *cmd)
{
	int	i;
	int	len;
	int	len_cmd;
	int	j;

	i = 0;
	while (i <= cmd->nb_pipe)
	{
		j = 0;
		len = 0;
		len_cmd = 0;
		cmd->tp_arg[i][0]->tp_val = NULL;
		while (cmd->tp_arg[i][j] && cmd->tp_arg[i][j]->value)
		{
			add_value_arg(&len, cmd, i, j);
			len_cmd++;
			j++;
		}
		parse_token_cmd_pipe(len, i, len_cmd, cmd);
		i++;
	}
}

void	execute_with_pipes(t_cmd *cmd, t_env *env[])
{
	cmd->tp_arg = malloc(sizeof(t_token **) * (cmd->nb_pipe + 1));
	if (!cmd->tp_arg)
		return ;
	copy_value_command_arg(cmd);
	count_cmd_per_arg(cmd);
	handle_pipe(cmd, env);
	free_commands(cmd);
	free(cmd->tp_arg);
}
