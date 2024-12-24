/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tambinin <tambinin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 13:38:53 by tambinin          #+#    #+#             */
/*   Updated: 2024/12/21 14:15:52 by tambinin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	update_env_var(const char *var_name, const char *value,
		t_env **env_list)
{
	size_t	len;
	t_env	*current;
	char	*new_data;

	len = ft_strlen(var_name);
	current = *env_list;
	while (current)
	{
		if (ft_strncmp(current->data, var_name, len) == 0
			&& current->data[len] == '=')
		{
			free(current->data);
			new_data = ft_strjoin3(var_name, "=", value);
			current->data = new_data;
			return ;
		}
		current = current->next;
	}
	new_data = ft_strjoin3(var_name, "=", value);
	add_env_node(env_list, new_data);
}

char	*ft_strjoin3(const char *s1, const char *s2, const char *s3)
{
	char	*result;
	size_t	len;

	len = ft_strlen(s1) + ft_strlen(s2) + ft_strlen(s3);
	result = malloc(len + 1);
	if (!result)
		return (NULL);
	ft_strlcpy(result, s1, len + 1);
	ft_strlcat(result, s2, len + 1);
	ft_strlcat(result, s3, len + 1);
	return (result);
}

int	update_add_loop(t_env *current, const char *var, size_t name_len,
		char *equal_pos)
{
	while (current)
	{
		if (ft_strncmp(current->data, var, name_len) == 0
			&& (current->data[name_len] == '='
				|| current->data[name_len] == '\0'))
		{
			if (equal_pos)
			{
				free(current->data);
				current->data = ft_strdup(var);
				set_st(0);
			}
			return (1);
		}
		current = current->next;
	}
	return (0);
}

void	update_or_add_variable(t_env **env, const char *var)
{
	size_t	name_len;
	char	*equal_pos;
	t_env	*current;

	current = *env;
	equal_pos = ft_strchr(var, '=');
	if (!equal_pos)
		name_len = ft_strlen(var);
	else
		name_len = equal_pos - var;
	if (update_add_loop(current, var, name_len, equal_pos) == 1)
		return ;
	add_env_node(env, (char *)var);
	set_st(0);
}
