/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_pipe_utils_second.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sandriam <sandriam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 14:23:27 by sandriam          #+#    #+#             */
/*   Updated: 2024/12/25 10:03:18 by sandriam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	init_index(int *index)
{
	index[0] = 0;
	index[1] = 0;
	index[2] = 0;
}

void	init_command_allocation(t_cmd *cmd, int *index)
{
	cmd->tp_arg[index[1]] = malloc(sizeof(t_token *) * (cmd->len_tokens
				+ 1));
	if (!cmd->tp_arg[index[1]])
		return ;
}

void	copy_token_to_command(t_cmd *cmd, int *i)
{
	cmd->tp_arg[i[1]][i[2]] = malloc(sizeof(t_token));
	if (!cmd->tp_arg[i[1]][i[2]])
		return ;
	cmd->tp_arg[i[1]][i[2]]->value = cmd->tokens[i[0]]->value;
	cmd->tp_arg[i[1]][i[2]]->type = cmd->tokens[i[0]]->type;
	cmd->tp_arg[i[1]][i[2]]->type_env = cmd->tokens[i[0]]->type_env;
	cmd->tp_arg[i[1]][i[2]]->type_delim = cmd->tokens[i[0]]->type_delim;
}

void	handle_pipe_and_finalize(t_cmd *cmd, int *index)
{
	cmd->tp_arg[index[1]][index[2]] = NULL;
	index[1]++;
	index[2] = 0;
}

void	copy_value_command_arg(t_cmd *cmd)
{
	int	index[3];

	init_index(index);
	while (index[0] < cmd->len_tokens)
	{
		if (cmd->tokens[index[0]]->type == PIPE)
			handle_pipe_and_finalize(cmd, index);
		else
		{
			if (index[2] == 0)
				init_command_allocation(cmd, index);
			copy_token_to_command(cmd, index);
			index[2]++;
		}
		index[0]++;
	}
	cmd->tp_arg[index[1]][index[2]] = NULL;
}
