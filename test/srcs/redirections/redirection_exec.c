/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_exec.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sandriam <sandriam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 16:35:37 by tambinin          #+#    #+#             */
/*   Updated: 2024/12/24 09:39:17 by sandriam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	handle_redir_output(t_token **tokens, int k)
{
	int	fd;

	fd = open(tokens[k + 1]->value, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (fd < 0)
	{
		set_st(1);
		perror(tokens[k + 1]->value);
		return (2);
	}
	dup2(fd, 1);
	close(fd);
	return (0);
}

int	is_last_stdin(t_token **tokens, int k, int len_tokens)
{
	int	i;

	i = k + 1;
	while (i < len_tokens)
	{
		if (tokens[i]->type == REDIR_HEREDOC || tokens[i]->type == REDIR_IN)
			return (1);
		i++;
	}
	return (0);
}

int	handle_redir_input(t_token **tokens, int k, int len_tokens)
{
	int	fd;

	fd = open(tokens[k + 1]->value, O_RDONLY);
	if (fd < 0)
	{
		set_st(1);
		perror(tokens[k + 1]->value);
		return (2);
	}
	if (!is_last_stdin(tokens, k, len_tokens))
		dup2(fd, 0);
	close(fd);
	return (0);
}

int	handle_redir_out_append(t_token **tokens, int k)
{
	int	fd;

	fd = open(tokens[k + 1]->value, O_WRONLY | O_APPEND | O_CREAT, 0777);
	if (fd < 0)
	{
		set_st(1);
		perror(tokens[k + 1]->value);
		return (2);
	}
	dup2(fd, 1);
	close(fd);
	return (0);
}

int	handle_redirections(t_token **tokens, int *k, int len_tokens)
{
	if (tokens[*k]->type == REDIR_OUT)
	{
		if (handle_redir_output(tokens, *k) != 0)
			return (2);
		(*k)++;
	}
	else if (tokens[*k]->type == REDIR_IN)
	{
		if (handle_redir_input(tokens, *k, len_tokens) != 0)
			return (2);
		(*k)++;
	}
	else if (tokens[*k]->type == REDIR_OUTPUT_APPEND)
	{
		if (handle_redir_out_append(tokens, *k) != 0)
			return (2);
		(*k)++;
	}
	return (0);
}
