/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sandriam <sandriam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 09:50:08 by sandriam          #+#    #+#             */
/*   Updated: 2024/12/24 09:50:10 by sandriam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	is_redirection(t_token **tokens)
{
	if (tokens[0]->type == REDIR_IN || tokens[0]->type == REDIR_OUT
		|| tokens[0]->type == REDIR_OUTPUT_APPEND
		|| tokens[0]->type == REDIR_HEREDOC)
		return (1);
	return (0);
}

int	other_error_one(int i, int len_tokens, t_token **tokens)
{
	if (tokens[i]->type == PIPE && i + 1 == len_tokens)
	{
		set_st(2);
		ft_putendl_fd("minishell: syntax error with unclosed '|'", 2);
		return (-1);
	}
	if (tokens[i]->type == REDIR_OUT && tokens[i + 1]->type == PIPE)
	{
		set_st(2);
		ft_putendl_fd("minishell: syntax error near unexpected token `|'", 2);
		return (-1);
	}
	if ((tokens[i]->type == REDIR_OUTPUT_APPEND
			|| tokens[i]->type == REDIR_HEREDOC || tokens[i]->type == REDIR_OUT
			|| tokens[i]->type == REDIR_IN) && tokens[i
			+ 1]->type == REDIR_OUTPUT_APPEND)
	{
		set_st(2);
		ft_putendl_fd("minishell: syntax error near unexpected token '>>'", 2);
		return (-1);
	}
	return (0);
}

int	other_error_two_out(int i, t_token **tokens)
{
	if ((tokens[i]->type == REDIR_OUTPUT_APPEND
			|| tokens[i]->type == REDIR_HEREDOC || tokens[i]->type == REDIR_OUT
			|| tokens[i]->type == REDIR_IN) && tokens[i + 1]->type == REDIR_OUT)
	{
		set_st(2);
		ft_putendl_fd("minishell: syntax error near unexpected token '>'", 2);
		return (-1);
	}
	return (0);
}

int	other_error_two(int i, t_token **tokens)
{
	if ((tokens[i]->type == REDIR_OUTPUT_APPEND
			|| tokens[i]->type == REDIR_HEREDOC || tokens[i]->type == REDIR_OUT
			|| tokens[i]->type == REDIR_IN) && tokens[i
			+ 1]->type == REDIR_HEREDOC)
	{
		set_st(2);
		ft_putendl_fd("minishell: syntax error near unexpected token '<<'", 2);
		return (-1);
	}
	if ((tokens[i]->type == REDIR_OUTPUT_APPEND
			|| tokens[i]->type == REDIR_HEREDOC || tokens[i]->type == REDIR_IN)
		&& tokens[i + 1]->type == PIPE)
	{
		set_st(2);
		ft_putendl_fd("minishell: syntax error near unexpected token '|'", 2);
		return (-1);
	}
	if (other_error_two_out(i, tokens) == -1)
		return (-1);
	return (0);
}

int	other_error_third(int i, t_token **tokens)
{
	if ((tokens[i]->type == REDIR_OUTPUT_APPEND
			|| tokens[i]->type == REDIR_HEREDOC || tokens[i]->type == REDIR_OUT
			|| tokens[i]->type == REDIR_IN) && tokens[i + 1]->type == REDIR_IN)
	{
		set_st(2);
		ft_putendl_fd("minishell: syntax error near unexpected token '<'", 2);
		return (-1);
	}
	if (tokens[i]->type == PIPE && tokens[i + 1]->type == PIPE)
	{
		set_st(2);
		ft_putendl_fd("minishell: syntax error near unexpected token '||'", 2);
		return (-1);
	}
	return (0);
}
