/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_heredoc_expand.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sandriam <sandriam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 09:16:35 by sandriam          #+#    #+#             */
/*   Updated: 2024/12/24 09:16:36 by sandriam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*expand_heredoc_pipe(char *input, t_env **env)
{
	char	*result;
	int		len;
	int		index[2];

	result = initialize_expand_result(env, input, &len, index);
	if (!result)
		return (NULL);
	while (input[index[0]])
	{
		if (handle_double_dollar_add(index, input, result))
			continue ;
		else if (input[index[0]] == '$' && input[index[0] + 1] != '\0'
			&& input[index[0] + 1] != '$' && ((input[index[0] + 1] != ' '
					&& ft_isalnum(input[index[0] + 1])) || input[index[0]
					+ 1] == '_'))
			process_env_variable(input, index, env, result);
		else
			result[index[1]++] = input[index[0]++];
	}
	result[len] = '\0';
	input = add_input_final(input, result);
	return (input);
}
