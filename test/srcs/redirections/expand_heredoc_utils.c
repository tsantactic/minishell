/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_heredoc_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sandriam <sandriam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 09:16:38 by sandriam          #+#    #+#             */
/*   Updated: 2024/12/24 09:16:39 by sandriam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	count_len_value(char *var_name, t_env **env, int *len)
{
	const char	*value;

	value = get_env_value(var_name, env);
	if (value)
		*len += ft_strlen(value);
}

int	handle_env_variable(int *i, int *len, char *input, t_env **env)
{
	int		var_env;
	char	*var_name;

	if (input[*i] == '$' && input[*i + 1] != '\0' && input[*i + 1] != '$'
		&& ((input[*i + 1] != ' ' && ft_isalnum(input[*i + 1])) || input[*i
				+ 1] == '_'))
	{
		var_env = 0;
		while (ft_isalnum(input[*i + 1 + var_env]) || input[*i + 1
				+ var_env] == '_')
			var_env++;
		var_name = malloc(sizeof(char) * (var_env + 1));
		if (!var_name)
			return (1);
		ft_strncpy(var_name, (char *)&input[*i + 1], var_env);
		var_name[var_env] = '\0';
		count_len_value(var_name, env, len);
		free(var_name);
		*i += var_env + 1;
		return (1);
	}
	return (0);
}

int	handle_digit_variable(int *i, char *input)
{
	if (input[*i] == '$' && ft_isdigit(input[*i + 1]))
	{
		*i += 2;
		return (1);
	}
	return (0);
}

int	handle_double_dollar(int *i, int *len, char *input)
{
	if (input[*i] == '$' && input[*i + 1] == '$')
	{
		*i += 2;
		*len += 2;
		return (1);
	}
	return (0);
}

int	count_len_expand_value_pipe(t_env **env, char *input)
{
	int	i;
	int	len;

	i = 0;
	len = 0;
	while (input[i] != '\0')
	{
		if (handle_double_dollar(&i, &len, input))
			continue ;
		else if (handle_digit_variable(&i, input))
			continue ;
		else if (handle_env_variable(&i, &len, input, env))
			continue ;
		else
		{
			len++;
			i++;
		}
	}
	return (len);
}
