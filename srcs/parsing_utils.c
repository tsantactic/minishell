/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sandriam <sandriam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 16:45:11 by sandriam          #+#    #+#             */
/*   Updated: 2024/11/10 11:15:00 by sandriam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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

/*count how many argument we will have per command*/
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
