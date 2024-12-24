/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stock_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tambinin <tambinin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 12:04:16 by sandriam          #+#    #+#             */
/*   Updated: 2024/12/22 15:51:43 by tambinin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	add_final_args(char *s, char *arg, t_cmd *cmd, int i);
void	add_after_space(t_cmd *cmd, char *s, int i, char *arg);
t_cmd	*init_variable(t_cmd *cmd);

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
