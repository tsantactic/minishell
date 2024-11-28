/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_and_expand.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sandriam <sandriam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 17:59:52 by sandriam          #+#    #+#             */
/*   Updated: 2024/11/27 18:05:18 by sandriam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/*expansion*/
const char	*get_env_value(const char *var, char **env)
{
	size_t	len;
	int		i;

	len = ft_strlen(var);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], var, len) == 0 && env[i][len] == '=')
			return (&env[i][len + 1]);
		i++;
	}
	return ("");
}
void	toggle_quotes(t_cmd *cmd, char ch)
{
	if (ch == '"' && !cmd->in_single_quote)
	{
		cmd->in_double_quote = !cmd->in_double_quote;
	}
	else if (ch == '\'' && !cmd->in_double_quote)
	{
		cmd->in_single_quote = !cmd->in_single_quote;
	}
}

void	handle_dollar_sign(const char *str, char *result,
		size_t *indices, t_cmd *cmd)
{
	int			var_len;
	char		*var_name;
	const char	*value;

	var_len = 0;
	while (ft_isalnum(str[indices[0] + 1 + var_len]) || str[indices[0] + 1
		+ var_len] == '_')
		var_len++;
	if (var_len > 0 && cmd->type_token != DELIMITER)
	{
		var_name = malloc(var_len + 1);
		ft_strncpy(var_name, (char *)&str[indices[0] + 1], var_len);
		var_name[var_len] = '\0';
		value = get_env_value(var_name, cmd->lst_env);
		if (value)
		{
			ft_strncpy(&result[indices[1]], (char *)value, ft_strlen(value));
			indices[1] += ft_strlen(value);
		}
		free(var_name);
		indices[0] += var_len;
	}
	else
		result[indices[1]++] = str[indices[0]];
}

void	init_all(t_cmd *cmd, size_t *indices)
{
	indices[1] = 0;
	indices[0] = 0;
	cmd->in_single_quote = 0;
	cmd->in_double_quote = 0;
}
void	add_double_quote(const char *str, size_t *indices, char *result, t_cmd *cmd)
{
	if (str[indices[0] + 1] == '$')
	{
		result[indices[1]++] = '$';
		result[indices[1]++] = '$';
		indices[0]++;
	}
	else
		handle_dollar_sign(str, result, indices, cmd);
}

void	skip_add_value(t_cmd *cmd, const char *str, size_t *indices,
		char *result)
{
	if (str[indices[0]] == '$' && !cmd->in_double_quote && (str[indices[0]
			+ 1] == '"' || str[indices[0] + 1] == '\''))
		indices[0]++;
	else if (str[indices[0] + 1] == '"')
	{
		result[indices[1]++] = str[indices[0]];
		indices[0]++;
	}
}
void	add_all(size_t *indices, char *result, const char *str, t_cmd *cmd)
{
	while (indices[0] < ft_strlen(str))
	{
		if ((str[indices[0]] == '"' && !cmd->in_single_quote)
			|| (str[indices[0]] == '\'' && !cmd->in_double_quote))
			toggle_quotes(cmd, str[indices[0]]);
		else if (str[indices[0]] == '$' && !cmd->in_single_quote)
		{
			if ((str[indices[0]] == '$' && !cmd->in_double_quote
					&& (str[indices[0] + 1] == '"' || str[indices[0]
						+ 1] == '\'')) || (str[indices[0] + 1] == '"'))
			{
				skip_add_value(cmd, str, indices, result);
				continue ;
			}
			else
			{
				if (str[indices[0] + 1] == '?')
				{
					printf("state : %d\n", set_st(-1));
				}
				add_double_quote(str, indices, result, cmd);
			}
		}
		else
			result[indices[1]++] = str[indices[0]];
		indices[0]++;
	}
}
char	*remove_quotes_and_expand(t_cmd *cmd, const char *str, char **env,
		t_token_type type)
{
	char	*result;
	size_t	indices[2];

	result = malloc(sizeof(char) * 5000);
	if (!result)
		return (NULL);
	init_all(cmd, indices);
	cmd->type_token = type;
	cmd->lst_env = env;
	add_all(indices, result, str, cmd);
	result[indices[1]] = '\0';
	return (result);
}
