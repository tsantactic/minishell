/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   type_args.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tambinin <tambinin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 15:46:48 by sandriam          #+#    #+#             */
/*   Updated: 2024/12/22 16:13:11 by tambinin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	possible_quote_dollar(char *s)
{
	int	i;
	int	is_ok;
	int	is_quote;

	i = 0;
	is_ok = 0;
	is_quote = 0;
	while (s[i])
	{
		if (s[i] == '"' || s[i] == '\'')
		{
			is_ok = 0;
			is_quote = 1;
		}
		if (s[i] == '$' && is_quote == 0)
			is_ok = 1;
		i++;
	}
	if (is_ok == 1 && is_quote == 0)
		return (0);
	if (is_ok == 0 && is_quote == 0)
		return (2);
	return (1);
}

int	possible_dollar(char *s)
{
	int	i;
	int	is_ok;
	int	is_quote;

	i = 0;
	is_ok = 0;
	is_quote = 0;
	if (s[0] == '\'')
		is_quote = 1;
	while (s[i])
	{
		if (s[i] == '$' && is_quote == 0)
			is_ok = 1;
		i++;
	}
	if (is_ok == 1 && is_quote == 0)
		return (0);
	return (1);
}

void	allocate_tokens(t_cmd *cmd)
{
	int	i;

	cmd->tokens = malloc(cmd->len_arg * sizeof(t_token *));
	if (!cmd->tokens)
		return ;
	i = 0;
	while (i < cmd->len_arg)
	{
		cmd->tokens[i] = malloc(sizeof(t_token));
		if (!cmd->tokens[i])
			return ;
		cmd->tokens[i]->value = ft_strdup(cmd->token_arg[i]);
		i++;
	}
}

void	handle_redirects_type(t_cmd *cmd, t_token *token)
{
	if (cmd->infile)
	{
		token->type = INPUT_FILE;
		cmd->infile = 0;
	}
	if (cmd->outfile)
	{
		token->type = OUTPUT_FILE;
		cmd->outfile = 0;
	}
	if (cmd->outapp)
	{
		token->type = OUTPUT_FILE_APPEND;
		cmd->outapp = 0;
	}
	if (cmd->heredoc)
	{
		token->type = DELIMITER;
		cmd->heredoc = 0;
	}
}
