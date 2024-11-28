/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexers.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sandriam <sandriam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 09:08:16 by sandriam          #+#    #+#             */
/*   Updated: 2024/11/27 18:48:08 by sandriam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
t_cmd	*move_quote(t_cmd *cmd, char **env)
{
	char	*cleaned_arg;
	cmd->is_empty = 0;
	cmd->index = 0;
	while (cmd->index < cmd->len_arg)
	{
		cleaned_arg = remove_quotes_and_expand(cmd,
				cmd->tokens[cmd->index]->value, env,
				cmd->tokens[cmd->index]->type);
		if (cmd->is_empty && cmd->index < cmd->len_arg && cmd->tokens[cmd->index
			- 1]->type == 2 && cmd->tokens[cmd->index]->type == 3 && cmd->tokens[cmd->index]->in_quote == IN_QUOTE)
		{
			cmd->tokens[cmd->index]->type = 1;
			cmd->is_empty = 0;
		}
		else if ((cmd->tokens[cmd->index]->type == 1 || cmd->tokens[cmd->index]->type == 3) && ft_strlen(cleaned_arg) == 0
			&& cmd->tokens[cmd->index]->in_quote == NO_QUOTE)
		{
			cmd->tokens[cmd->index]->type = 2;
			cmd->is_empty = 1;
		}
		free(cmd->tokens[cmd->index]->value);
		cmd->tokens[cmd->index]->value = cleaned_arg;
		cmd->index++;
	}
	cmd->is_cmd = 0;
	cmd->index = 0;
	while (cmd->index < cmd->len_arg)
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
		cmd->index++;
	}
	return (cmd);
}

void initialise_all(t_cmd *cmd, int i)
{
    cmd->len_arg = i;
    cmd->tokens = NULL;
	cmd->len_tokens = 0;
}

int lexing_arg(char *line, t_cmd *cmd, char **env)
{
    int i;

    i = count_arg(line, cmd);
    initialise_all(cmd, i);
    if (i < 0)
	{
		if (i == -1)
		{
			ft_putendl_fd("minishell: syntax error with unclosed quotes", 2);
			return (-2);
		}
		else if (i == -2)
		{
			ft_putendl_fd("minishell: syntax error near unexpected token `|'", 2);
			return (-1);
		}
	}
	else
	{
        stock_arg(line, cmd);
        cmd->token_arg = malloc(sizeof(char *) * (cmd->len_arg + 1));
		if (!cmd->token_arg)
			return (-1);
		stock_token(cmd);
        type_token(cmd);
        cmd = move_quote(cmd, env);
		for (int i = 0; i < cmd->len_tokens; i++)
		{
			printf("Token Value: [%s], Type: %d\n", cmd->tokens[i]->value,
				cmd->tokens[i]->type);
		}
		return (0);
    }
    return (-1);
}