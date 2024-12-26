/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_pipe_util_third.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sandriam <sandriam@student.42antanana      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 15:27:28 by sandriam          #+#    #+#             */
/*   Updated: 2024/12/24 15:59:27 by sandriam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	init_value_lenght(int len, int i, int len_cmd, t_cmd *cmd)
{
	cmd->tp_arg[i][0]->len_cmd_arg = len;
	cmd->tp_arg[i][0]->total_len = len_cmd;
}

void	parse_token_cmd_pipe(int len, int i, int len_cmd, t_cmd *cmd)
{
	int	j;
	int	k;

	k = 0;
	init_value_lenght(len, i, len_cmd, cmd);
	cmd->tp_arg[i][0]->tp_args = malloc(sizeof(char *)
			* (cmd->tp_arg[i][0]->len_cmd_arg + 1));
	j = 0;
	while (cmd->tp_arg[i][j] && cmd->tp_arg[i][j]->value)
	{
		if (cmd->tp_arg[i][j]->type == ARG || cmd->tp_arg[i][j]->type == CMD)
		{
			if (cmd->tp_arg[i][j]->type_env == IS_ENV
				&& ft_strchr(cmd->tp_arg[i][j]->value, ' ') != 0)
				cmd->tp_arg[i][0]->tp_val = add_value(cmd->tp_arg[i][j]->value,
						&k, cmd->tp_arg[i][0]->tp_args,
						cmd->tp_arg[i][0]->tp_val);
			else
				cmd->tp_arg[i][0]->tp_args[k++] = ft_strdup(
						cmd->tp_arg[i][j]->value);
		}
		j++;
	}
	cmd->tp_arg[i][0]->tp_args[k] = NULL;
}
