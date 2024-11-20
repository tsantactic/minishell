/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stock_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sandriam <sandriam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 12:04:16 by sandriam          #+#    #+#             */
/*   Updated: 2024/11/12 17:12:04 by sandriam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	is_space(int i, char *s)
{
	while ((s[i] == ' ' || s[i] == '\n' || s[i] == '\t') && s[i])
		i++;
	return (i);
}

void add_arg(t_cmd *cmd, char *arg)
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
			return;
		add_arg(cmd, arg);
	}
	arg =NULL;
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
				return;
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
/*just stock argument separate with space*/
void	stock_arg(char *s, t_cmd *cmd)
{
	int		i;
	char	*arg;

	i = 0;
	init_variable(cmd);
	arg = NULL;
	while (s[i])
	{
		if (s[i] == '\'' && !cmd->in_double_quote)
		{
			cmd->in_single_quote = !cmd->in_single_quote;
			i++;
			continue ;
		}
		if (s[i] == '\"' && !cmd->in_single_quote)
		{
			cmd->in_double_quote = !cmd->in_double_quote;
			i++;
			continue ;
		}
		add_after_space(cmd, s, i, arg);
		i++;
	}
	add_final_args(s, arg, cmd, i);
}
