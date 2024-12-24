/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sandriam <sandriam@student.42antanana      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 10:38:31 by sandriam          #+#    #+#             */
/*   Updated: 2024/12/23 12:13:01 by sandriam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	check_error_arg(t_token **tokens, int len_tokens)
{
	int	i;

	if (len_tokens == 1 && (is_redirection(tokens)))
	{
		set_st(2);
		printf("minishell: syntax error near unexpected token `newline'\n");
		return (-1);
	}
	else
	{
		i = 0;
		while (i < len_tokens)
		{
			if (check_syntax_one(tokens, len_tokens, i) == -2)
				return (-2);
			if (other_error_one(i, len_tokens, tokens) == -1
				|| other_error_two(i, tokens) == -1 || other_error_third(i,
					tokens) == -1)
				return (-1);
			i++;
		}
	}
	return (1);
}
