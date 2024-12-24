/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_and_expand.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tambinin <tambinin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 17:59:52 by sandriam          #+#    #+#             */
/*   Updated: 2024/12/13 16:48:04 by tambinin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*expansion*/
const char	*get_env_value(const char *var, t_env **env_list)
{
	size_t	len;
	t_env	*current;

	len = ft_strlen(var);
	current = *env_list;
	while (current)
	{
		if (ft_strncmp(current->data, var, len) == 0 && current->data[len] == '=')
			return (&current->data[len + 1]);
		current = current->next;
	}
	return (NULL);
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
		value = get_env_value(var_name, cmd->env_lst);
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
	if (ft_isdigit(str[indices[0] + 1]) == 1)
	{
		indices[0]++;
	}
	else if (str[indices[0] + 1] == '$')
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

void expand_status(size_t *indices, char *result)
{
	char *tmp = ft_itoa(set_st(-1));
	size_t i = -1;
	while (++i < ft_strlen(tmp))
	{
		result[indices[1]++] = tmp[i];
	}
	indices[0]++;
	free(tmp);
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
				if (str[indices[0] + 1] == '?' && cmd->type_token != DELIMITER)
				{
					expand_status(indices, result);					
				}
				else
					add_double_quote(str, indices, result, cmd);
			}
		}
		else
			result[indices[1]++] = str[indices[0]];
		indices[0]++;
	}
}
char	*remove_quotes_and_expand(t_cmd *cmd, const char *str,  t_env **env_list,
		t_token_type type)
{
	char	*result;
	size_t	indices[2];

	result = malloc(sizeof(char) * 5000);
	if (!result)
		return (NULL);
	init_all(cmd, indices);
	cmd->type_token = type;
	cmd->env_lst = env_list;
	add_all(indices, result, str, cmd);
	result[indices[1]] = '\0';
	return (result);
}
