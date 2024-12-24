/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenisation.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tambinin <tambinin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 14:53:33 by sandriam          #+#    #+#             */
/*   Updated: 2024/12/22 16:09:14 by tambinin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	contains_quotes(const char *arg)
{
	size_t	i;

	i = 0;
	if (arg[0] == '\0')
		return (0);
	while (i < ft_strlen(arg))
	{
		if ((arg[i] == '\'' && arg[i + 1] == '"') || (arg[i] == '"' && arg[i
					+ 1] == '\'') || ((arg[i] != '"' && arg[i] != '\'')))
			return (0);
		i++;
	}
	return (1);
}

int	redirection_contains_quotes(const char *arg)
{
	size_t	i;

	i = 0;
	while (arg[i])
	{
		if (arg[i] == '<' || arg[i] == '>' || arg[i] == '|')
		{
			return (1);
		}
		i++;
	}
	return (0);
}

int	is_quote(char c)
{
	return (c == '"' || c == '\'');
}

int	check_quote(char arg, int in_quotes)
{
	if (is_quote(arg))
	{
		if (in_quotes == 0)
		{
			if (arg == '"')
				in_quotes = 2;
			else
				in_quotes = 1;
		}
		else if ((in_quotes == 1 && arg == '\'') || (in_quotes == 2
				&& arg == '"'))
			in_quotes = 0;
	}
	return (in_quotes);
}

void	add_final_arg(t_cmd *cmd, const char *arg, size_t start)
{
	if (start < ft_strlen(arg))
	{
		cmd->token_arg[cmd->arg_index] = ft_strdup(arg + start);
		if (!cmd->token_arg[cmd->arg_index])
			return ;
		cmd->arg_index++;
	}
}
