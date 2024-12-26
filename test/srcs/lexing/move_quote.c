/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move_quote.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sandriam <sandriam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 15:02:54 by tambinin          #+#    #+#             */
/*   Updated: 2024/12/24 14:30:10 by sandriam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_cmd	*handle_empty_or_quote(t_cmd *cmd, char *cleaned_arg, int index)
{
	if (cmd->is_empty && index < cmd->len_arg && cmd->tokens[index
			- 1]->type == 2 && cmd->tokens[index]->type == 3
		&& (cmd->tokens[index]->in_quote == SIMPLE
			|| cmd->tokens[index]->in_quote == IN_QUOTE))
	{
		cmd->tokens[index]->type = 1;
		cmd->is_empty = 0;
	}
	else if ((cmd->tokens[index]->type == 1 || cmd->tokens[index]->type == 3)
		&& ft_strlen(cleaned_arg) == 0
		&& cmd->tokens[index]->in_quote == NO_QUOTE)
	{
		cmd->tokens[index]->type = 2;
		cmd->is_empty = 1;
	}
	return (cmd);
}

void	process_token_quote(t_cmd *cmd, t_env *env_list, int index)
{
	char	*cleaned_arg;

	check_quote_delimiter(cmd->tokens[index]->value,
		&cmd->tokens[index]->type_delim);
	cleaned_arg = remove_quotes_and_expand(cmd, cmd->tokens[index]->value,
			&env_list, cmd->tokens[index]->type);
	cmd = handle_empty_or_quote(cmd, cleaned_arg, index);
	free(cmd->tokens[index]->value);
	cmd->tokens[index]->value = cleaned_arg;
}

t_cmd	*move_quote(t_cmd *cmd, t_env *env_list)
{
	cmd->is_empty = 0;
	cmd->index = -1;
	while (++cmd->index < cmd->len_arg)
	{
		process_token_quote(cmd, env_list, cmd->index);
	}
	last_type(cmd);
	return (cmd);
}

int	lexing_arg(char *line, t_cmd *cmd, t_env **env)
{
	int	len_arg;

	len_arg = count_arg(line, cmd);
	initialise_all(cmd, len_arg);
	if (len_arg < 0)
		return (handle_lexing_error(len_arg));
	stock_arg(line, cmd);
	cmd->token_arg = malloc(sizeof(char *) * (cmd->len_arg + 1));
	if (!cmd->token_arg)
		return (-1);
	return (process_tokens(cmd, *env));
}
