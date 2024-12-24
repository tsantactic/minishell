/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tambinin <tambinin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 15:30:43 by tambinin          #+#    #+#             */
/*   Updated: 2024/12/22 17:14:12 by tambinin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	toggle_quote(char c, t_cmd *cmd)
{
	if (c == '"' && !cmd->in_single_quote)
		cmd->in_double_quote = !cmd->in_double_quote;
	else if (c == '\'' && !cmd->in_double_quote)
		cmd->in_single_quote = !cmd->in_single_quote;
}

int	handle_redirects(char *s, int *i)
{
	if (s[*i] == '>' && s[*i + 1] == '>')
		(*i)++;
	if (s[*i] == '<' && s[*i + 1] == '<')
		(*i)++;
	return (1);
}
