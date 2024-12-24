/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenisation2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tambinin <tambinin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 15:53:57 by tambinin          #+#    #+#             */
/*   Updated: 2024/12/22 16:02:28 by tambinin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	add_redirection(t_cmd *cmd, int i, const char *arg, char *redir)
{
	if (arg[i] == '<' && arg[i + 1] == '<')
	{
		cmd->token_arg[cmd->arg_index++] = ft_strdup("<<");
		i++;
	}
	else if (arg[i] == '>' && arg[i + 1] == '>')
	{
		cmd->token_arg[cmd->arg_index++] = ft_strdup(">>");
		i++;
	}
	else
	{
		redir[0] = arg[i];
		redir[1] = '\0';
		cmd->token_arg[cmd->arg_index++] = ft_strdup(redir);
	}
	return (i);
}

void	convert_redirection(const char *arg, t_cmd *cmd)
{
	size_t	i;
	size_t	start;
	int		in_quotes;
	char	redir[3];

	i = 0;
	start = 0;
	in_quotes = 0;
	while (i < ft_strlen(arg))
	{
		in_quotes = check_quote(arg[i], in_quotes);
		if (in_quotes == 0 && (arg[i] == '<' || arg[i] == '>' || arg[i] == '|'))
		{
			if (i > start)
			{
				cmd->token_arg[cmd->arg_index++] = ft_strndup(arg + start, i
						- start);
			}
			i = add_redirection(cmd, i, arg, redir);
			start = i + 1;
		}
		i++;
	}
	add_final_arg(cmd, arg, start);
}

void	free_token_arg(t_cmd *cmd, int i)
{
	int	j;

	j = 0;
	while (j < i)
	{
		free(cmd->token_arg[j]);
		j++;
	}
	free(cmd->token_arg);
	cmd->token_arg = NULL;
}

void	stock_token(t_cmd *cmd)
{
	int	i;
	int	len;

	i = 0;
	len = cmd->arg_index;
	cmd->arg_index = 0;
	while (i < len)
	{
		if (contains_quotes(cmd->args[i]) == 1)
			cmd->token_arg[cmd->arg_index++] = ft_strdup("\"\"");
		else if (redirection_contains_quotes(cmd->args[i]) == 1)
			convert_redirection(cmd->args[i], cmd);
		else
			cmd->token_arg[cmd->arg_index++] = ft_strdup(cmd->args[i]);
		if (!cmd->token_arg[cmd->arg_index - 1])
		{
			free_token_arg(cmd, cmd->arg_index);
			return ;
		}
		i++;
	}
	cmd->token_arg[cmd->len_arg] = NULL;
}
