/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   type_args3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tambinin <tambinin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 16:10:29 by tambinin          #+#    #+#             */
/*   Updated: 2024/12/22 16:20:28 by tambinin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	set_type_quote(char *value, t_token *token);
void	action_one_of_four(int action, t_token *token, t_cmd *cmd);
void	action_five_of_nine(int action, t_token *token, t_cmd *cmd);
int		handle_action(int action, t_cmd *cmd, t_token *token, char *value);
void	allocate_tokens(t_cmd *cmd);

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
