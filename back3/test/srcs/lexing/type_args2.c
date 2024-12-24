/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   type_args2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tambinin <tambinin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 16:10:54 by tambinin          #+#    #+#             */
/*   Updated: 2024/12/22 16:20:12 by tambinin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int		possible_dollar(char *s);
int		possible_quote_dollar(char *s);
void	handle_redirects_type(t_cmd *cmd, t_token *token);

void	set_type_quote(char *value, t_token *token)
{
	if (possible_dollar(value) == 0)
		token->type_env = IS_ENV;
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
