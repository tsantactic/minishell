/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sandriam <sandriam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 09:16:42 by sandriam          #+#    #+#             */
/*   Updated: 2024/12/24 09:16:43 by sandriam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*add_input_final(char *input, char *result)
{
	int	i;

	input = malloc(sizeof(char) * ft_strlen(result) + 1);
	i = 0;
	while (result[i])
	{
		input[i] = result[i];
		i++;
	}
	free(result);
	input[i] = '\0';
	return (input);
}

int	handle_double_dollar_add(int *index, char *input, char *result)
{
	if (input[index[0]] == '$' && input[index[0] + 1] == '$')
	{
		result[index[1]++] = '$';
		result[index[1]++] = '$';
		index[0] += 2;
		return (1);
	}
	else if (input[index[0]] == '$' && ft_isdigit(input[index[0] + 1]))
	{
		index[0] += 2;
		return (1);
	}
	return (0);
}

char	*initialize_expand_result(t_env **env, char *input, int *len,
		int *index)
{
	char	*result;

	*len = count_len_expand_value_pipe(env, input);
	index[0] = 0;
	index[1] = 0;
	result = malloc(sizeof(char) * (*len + 1));
	if (!result)
		return (NULL);
	return (result);
}

int	count_var(char *input, int i)
{
	int	var_env;

	var_env = 0;
	while (ft_isalnum(input[i + 1 + var_env]) || input[i + 1 + var_env] == '_')
		var_env++;
	return (var_env);
}

void	process_env_variable(char *input, int *index, t_env **env, char *result)
{
	int			var_env;
	char		*var_name;
	const char	*value;

	var_env = count_var(input, index[0]);
	var_name = malloc(sizeof(char) * var_env + 1);
	ft_strncpy(var_name, (char *)&input[index[0] + 1], var_env);
	var_name[var_env] = '\0';
	value = get_env_value(var_name, env);
	if (value)
	{
		ft_strncpy(&result[index[1]], (char *)value, ft_strlen(value));
		index[1] += ft_strlen(value);
	}
	free(var_name);
	index[0] += var_env + 1;
}
