/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sandriam <sandriam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 11:18:30 by tambinin          #+#    #+#             */
/*   Updated: 2024/12/24 09:50:23 by sandriam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	add_env_node(t_env **head, char *value)
{
	t_env	*new_node;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
	{
		perror("Failed to allocate memory for env node");
		return ;
	}
	new_node->data = ft_strdup(value);
	new_node->next = *head;
	*head = new_node;
}

void	free_env_list(t_env *lst)
{
	t_env	*tmp;

	while (lst)
	{
		tmp = lst->next;
		free(lst);
		lst = tmp;
	}
}

t_env	*copy_env_to_list(char **env)
{
	t_env	*env_list;
	int		i;

	env_list = NULL;
	i = 0;
	while (env[i])
	{
		add_env_node(&env_list, env[i]);
		i++;
	}
	return (env_list);
}

char	**fill_env_array(t_env *lst, char **env_array, int count)
{
	int		i;
	int		j;
	t_env	*current;

	current = lst;
	i = 0;
	while (i < count)
	{
		env_array[i] = ft_strdup(current->data);
		if (!env_array)
		{
			j = 0;
			while (j < i)
			{
				free(env_array[j]);
				j++;
			}
			free(env_array);
			return (NULL);
		}
		current = current->next;
		i++;
	}
	env_array[count] = NULL;
	return (env_array);
}

char	**env_list_to_array(t_env *lst)
{
	int		count;
	t_env	*current;
	char	**env_array;

	count = 0;
	current = lst;
	while (current)
	{
		count++;
		current = current->next;
	}
	env_array = malloc(sizeof(char *) * (count + 1));
	if (!env_array)
	{
		perror("Failed to allocate memory");
		return (NULL);
	}
	return (fill_env_array(lst, env_array, count));
}
