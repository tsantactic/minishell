/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_utils3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sandriam <sandriam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 09:50:15 by sandriam          #+#    #+#             */
/*   Updated: 2024/12/24 09:50:16 by sandriam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	set_index_redir(int i, t_token **tokens, int len_tokens)
{
	if ((tokens[i]->type == REDIR_IN || tokens[i]->type == REDIR_OUT
			|| tokens[i]->type == REDIR_OUTPUT_APPEND
			|| tokens[i]->type == REDIR_HEREDOC) && i + 1 == len_tokens)
	{
		return (i);
	}
	if (tokens[i]->type == PIPE && i + 1 == len_tokens)
	{
		return (i);
	}
	if (tokens[i]->type == REDIR_OUT && tokens[i + 1]->type == PIPE)
	{
		return (i);
	}
	if ((tokens[i]->type == REDIR_OUTPUT_APPEND
			|| tokens[i]->type == REDIR_HEREDOC || tokens[i]->type == REDIR_OUT
			|| tokens[i]->type == REDIR_IN) && tokens[i
			+ 1]->type == REDIR_OUTPUT_APPEND)
	{
		return (i);
	}
	return (-2);
}

int	set_index_redir_other(int i, t_token **tokens)
{
	if ((tokens[i]->type == REDIR_OUTPUT_APPEND
			|| tokens[i]->type == REDIR_HEREDOC || tokens[i]->type == REDIR_OUT
			|| tokens[i]->type == REDIR_IN) && tokens[i
			+ 1]->type == REDIR_HEREDOC)
	{
		return (i);
	}
	if ((tokens[i]->type == REDIR_OUTPUT_APPEND
			|| tokens[i]->type == REDIR_HEREDOC || tokens[i]->type == REDIR_IN)
		&& tokens[i + 1]->type == PIPE)
	{
		return (i);
	}
	if ((tokens[i]->type == REDIR_OUTPUT_APPEND
			|| tokens[i]->type == REDIR_HEREDOC || tokens[i]->type == REDIR_OUT
			|| tokens[i]->type == REDIR_IN) && tokens[i + 1]->type == REDIR_OUT)
	{
		return (i);
	}
	return (-2);
}

int	set_index_syntax(t_token **tokens, int len_tokens)
{
	int	i;

	i = 0;
	while (i < len_tokens)
	{
		if (set_index_redir(i, tokens, len_tokens) != -2)
			return (i);
		if (set_index_redir_other(i, tokens) != -2)
			return (i);
		if ((tokens[i]->type == REDIR_OUTPUT_APPEND
				|| tokens[i]->type == REDIR_HEREDOC
				|| tokens[i]->type == REDIR_OUT || tokens[i]->type == REDIR_IN)
			&& tokens[i + 1]->type == REDIR_IN)
		{
			return (i);
		}
		if (tokens[i]->type == PIPE && tokens[i + 1]->type == PIPE)
		{
			return (i);
		}
		i++;
	}
	return (i);
}
