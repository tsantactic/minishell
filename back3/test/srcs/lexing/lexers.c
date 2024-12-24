/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexers.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tambinin <tambinin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 09:08:16 by sandriam          #+#    #+#             */
/*   Updated: 2024/12/22 15:26:04 by tambinin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	last_type(t_cmd *cmd)
{
	cmd->is_cmd = 0;
	cmd->index = -1;
	while (++cmd->index < cmd->len_arg)
	{
		if (cmd->tokens[cmd->index]->type == PIPE)
		{
			cmd->is_cmd = 0;
		}
		else if (cmd->tokens[cmd->index]->type == 1)
		{
			if (cmd->is_cmd == 0)
				cmd->is_cmd = 1;
			else
				cmd->tokens[cmd->index]->type = 3;
		}
	}
}

void	check_quote_delimiter(char *value, t_token_type *type_quote_delim)
{
	int	i;

	i = 0;
	*type_quote_delim = -1;
	while (i < (int)ft_strlen(value))
	{
		if (value[i] == '\'' || value[i] == '"')
		{
			*type_quote_delim = DELIMITER_WITH_QUOTE;
		}
		i++;
	}
	if (*type_quote_delim != DELIMITER_WITH_QUOTE)
		*type_quote_delim = DELIMITER_NO_QUOTE;
}

void	initialise_all(t_cmd *cmd, int i)
{
	cmd->len_arg = i;
	cmd->tokens = NULL;
	cmd->len_tokens = 0;
}

int	handle_lexing_error(int error_code)
{
	set_st(2);
	if (error_code == -1)
	{
		ft_putendl_fd("minishell: syntax error with unclosed quotes", 2);
	}
	else if (error_code == -2)
	{
		ft_putendl_fd("minishell: syntax error near unexpected token `|'", 2);
	}
	return (-2);
}

int	process_tokens(t_cmd *cmd, t_env *env)
{
	stock_token(cmd);
	type_token(cmd);
	cmd = move_quote(cmd, env);
	return (0);
}
