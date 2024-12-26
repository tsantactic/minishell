/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_with_pipe.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sandriam <sandriam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 11:23:13 by tambinin          #+#    #+#             */
/*   Updated: 2024/12/24 15:49:13 by sandriam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	parse_exec_redir_pipe(t_token **tp_arg, int len)
{
	if (contains_redir_pipe(tp_arg, len))
	{
		if (redirection_exec_pipe(tp_arg, len) == 2)
			return (1);
	}
	return (0);
}

int	contains_redir_pipe(t_token **token, int len)
{
	int	k;

	k = 0;
	while (k < len)
	{
		if (token[k]->type == REDIR_IN)
			return (1);
		if (token[k]->type == REDIR_OUT)
			return (1);
		if (token[k]->type == REDIR_OUTPUT_APPEND)
			return (1);
		k++;
	}
	return (0);
}
