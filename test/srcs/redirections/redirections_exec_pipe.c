/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_exec_pipe.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tambinin <tambinin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 16:03:46 by tambinin          #+#    #+#             */
/*   Updated: 2024/12/23 16:33:21 by tambinin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	handle_redir_out(t_token **tokens, int k)
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

int	handle_redir_in(t_token **tokens, int k, int len)
{
	int	fd;
	int	i;
	int	is_last_stdin;

	fd = open(tokens[k + 1]->value, O_RDONLY);
	if (fd < 0)
	{
		set_st(1);
		perror(tokens[k + 1]->value);
		return (2);
	}
	i = k + 1;
	is_last_stdin = 0;
	while (i < len)
	{
		if (tokens[i]->type == REDIR_HEREDOC || tokens[i]->type == REDIR_IN)
			is_last_stdin = 1;
		i++;
	}
	if (is_last_stdin == 0)
		dup2(fd, 0);
	close(fd);
	return (0);
}

int	handle_redir_output_append(t_token **tokens, int k)
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

int	handle_redirection(t_token **tokens, int *k, int len)
{
	if (tokens[*k]->type == REDIR_OUT)
	{
		if (handle_redir_out(tokens, *k) != 0)
			return (2);
		(*k)++;
	}
	else if (tokens[*k]->type == REDIR_IN)
	{
		if (handle_redir_in(tokens, *k, len) != 0)
			return (2);
		(*k)++;
	}
	else if (tokens[*k]->type == REDIR_OUTPUT_APPEND)
	{
		if (handle_redir_output_append(tokens, *k) != 0)
			return (2);
		(*k)++;
	}
	return (0);
}

int	redirection_exec_pipe(t_token **tokens, int len)
{
	int	k;

	k = 0;
	while (k < len)
	{
		if (handle_redirection(tokens, &k, len) != 0)
			return (2);
		k++;
	}
	return (0);
}
