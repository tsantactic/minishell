/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   type_args.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tambinin <tambinin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 15:46:48 by sandriam          #+#    #+#             */
/*   Updated: 2024/12/13 16:48:17 by tambinin         ###   ########.fr       */
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
void	set_type_quote(char *value, t_token *token)
{
	if (possible_dollar(value) == 0)
	{
		token->type_env = IS_ENV;
	}
	else
		token->type_env = -1;
	if (possible_quote_dollar(value) == 1)
		token->in_quote = IN_QUOTE;
	else if (possible_quote_dollar(value) == 2)
		token->in_quote = SIMPLE;
	else
		token->in_quote = NO_QUOTE;
}
int	handle_action(int action, t_cmd *cmd, t_token *token, char *value)
{
	set_type_quote(value, token);
	if (ft_strcmp(value, "|") == 0)
		action = 1;
	else if (ft_strcmp(value, "<<") == 0)
		action = 2;
	else if (ft_strcmp(value, ">") == 0)
		action = 3;
	else if (ft_strcmp(value, "<") == 0)
		action = 4;
	else if (ft_strcmp(value, ">>") == 0)
		action = 5;
	else if (ft_strcmp(value, "\"\"") == 0 && !cmd->outfile && !cmd->infile
		&& !cmd->outapp && !cmd->heredoc && !cmd->is_cmd)
		action = 6;
	else if (cmd->is_cmd == 1 && !cmd->outfile && !cmd->infile && !cmd->outapp
		&& !cmd->heredoc)
		action = 7;
	else if (cmd->outfile || cmd->infile || cmd->outapp || cmd->heredoc)
		action = 8;
	else
		action = 9;
	return (action);
}

void	action_one_of_four(int action, t_token *token, t_cmd *cmd)
{
	if (action == 1)
	{
		token->type = PIPE;
		cmd->is_cmd = 0;
	}
	if (action == 2)
	{
		token->type = REDIR_HEREDOC;
		cmd->heredoc = 1;
	}
	if (action == 3)
	{
		token->type = REDIR_OUT;
		cmd->outfile = 1;
	}
	if (action == 4)
	{
		token->type = REDIR_IN;
		cmd->infile = 1;
	}
}

void	action_five_of_nine(int action, t_token *token, t_cmd *cmd)
{
	if (action == 5)
	{
		token->type = REDIR_OUTPUT_APPEND;
		cmd->outapp = 1;
	}
	if (action == 6)
		token->type = CMD;
	if (action == 7)
		token->type = ARG;
	if (action == 8)
		handle_redirects_type(cmd, token);
	if (action == 9)
	{
		token->type = CMD;
		cmd->is_cmd = 1;
	}
}

void	set_token_type(int action, t_cmd *cmd, t_token *token, char *value)
{
	set_type_quote(value, token);
	action_one_of_four(action, token, cmd);
	action_five_of_nine(action, token, cmd);
}

void	search_token_properties(t_cmd *cmd)
{
	int	i;
	int	action;

	action = 0;
	i = 0;
	while (i < cmd->len_arg)
	{
		action = handle_action(action, cmd, cmd->tokens[i], cmd->token_arg[i]);
		set_token_type(action, cmd, cmd->tokens[i], cmd->token_arg[i]);
		i++;
	}
}
void	init_flag(t_cmd *cmd)
{
	cmd->is_cmd = 0;
	cmd->infile = 0;
	cmd->outfile = 0;
	cmd->outapp = 0;
	cmd->heredoc = 0;
}
void	type_token(t_cmd *cmd)
{
	init_flag(cmd);
	allocate_tokens(cmd);
	search_token_properties(cmd);
	cmd->len_tokens = cmd->len_arg;
}
