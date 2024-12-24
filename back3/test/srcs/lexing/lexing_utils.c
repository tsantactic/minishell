/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tambinin <tambinin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 09:21:49 by sandriam          #+#    #+#             */
/*   Updated: 2024/12/22 15:32:49 by tambinin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	init_var_quote(t_cmd *cmd)
{
	cmd->in_double_quote = 0;
	cmd->in_single_quote = 0;
	cmd->in_word = 0;
}

static int	iss_space(t_cmd *cmd, char *s)
{
	int	i;

	i = 0;
	init_var_quote(cmd);
	while (s[i] == ' ' || s[i] == '\n' || s[i] == '\t')
		i++;
	return (i);
}

int	count_all(int len, t_cmd *cmd)
{
	if ((cmd->in_double_quote || cmd->in_single_quote))
		return (-1);
	return (len + cmd->in_word);
}

int	count_word(char *s, int *i, int *len, t_cmd *cmd)
{
	if (isspace(s[*i]) && !cmd->in_double_quote && !cmd->in_single_quote)
	{
		*len += cmd->in_word;
		cmd->in_word = 0;
	}
	else if ((s[*i] == '|' || s[*i] == '<' || s[*i] == '>')
		&& !cmd->in_double_quote && !cmd->in_single_quote)
	{
		*len += cmd->in_word;
		cmd->in_word = 0;
		if (handle_redirects(s, i) < 0)
			return (handle_redirects(s, i));
		(*len)++;
	}
	else
		cmd->in_word = 1;
	return (*len);
}

int	count_arg(char *s, t_cmd *cmd)
{
	int	i;
	int	len;

	i = iss_space(cmd, s);
	len = 0;
	if ((s[i] == '|' && s[i + 1] != '|') || (s[i] == '|' && s[i + 1] == ' '))
		return (-2);
	while (s[i])
	{
		toggle_quote(s[i], cmd);
		len = count_word(s, &i, &len, cmd);
		i++;
	}
	return (count_all(len, cmd));
}
