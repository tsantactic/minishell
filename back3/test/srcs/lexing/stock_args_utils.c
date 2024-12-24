/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stock_args_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tambinin <tambinin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 15:47:31 by tambinin          #+#    #+#             */
/*   Updated: 2024/12/22 15:52:45 by tambinin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	add_arg(t_cmd *cmd, char *arg)
{
	if (cmd->arg_index < cmd->len_arg)
		cmd->args[cmd->arg_index++] = arg;
	else
		free(arg);
}

void	add_final_args(char *s, char *arg, t_cmd *cmd, int i)
{
	if (cmd->start < i)
	{
		arg = ft_strndup(&s[cmd->start], i - cmd->start);
		if (!arg)
			return ;
		add_arg(cmd, arg);
	}
	arg = NULL;
	cmd->args[cmd->arg_index] = NULL;
}

void	add_after_space(t_cmd *cmd, char *s, int i, char *arg)
{
	if ((s[i] == ' ' || s[i] == '\t' || s[i] == '\n') && !cmd->in_single_quote
		&& !cmd->in_double_quote)
	{
		if (cmd->start < i)
		{
			arg = ft_strndup(&s[cmd->start], i - cmd->start);
			if (!arg)
				return ;
			add_arg(cmd, arg);
		}
		cmd->start = i + 1;
	}
}

t_cmd	*init_variable(t_cmd *cmd)
{
	cmd->start = 0;
	cmd->in_single_quote = 0;
	cmd->in_double_quote = 0;
	cmd->arg_index = 0;
	return (cmd);
}
