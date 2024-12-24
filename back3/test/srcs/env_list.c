/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sandriam <sandriam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 11:18:30 by tambinin          #+#    #+#             */
/*   Updated: 2024/11/29 10:22:39 by sandriam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// Pour ajouter un noeud a la liste
void	add_env_node(t_env **head, char *value)
{
	t_env	*new_node = malloc(sizeof(t_env));
	if (!new_node)
	{
		perror("Failed to allocate memory for env node");
		return ;
	}
	new_node->data = ft_strdup(value); // copier la valeur
	//free(value);
	new_node->next = *head; // ajouter au debut de la liste
	*head = new_node; // Maj la tete de liste
}
// Pour liberer la memoire de la liste
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
	t_env	*env_list = NULL; // init a NULL
	int		i = 0;

	while (env[i])
	{
		add_env_node(&env_list, env[i]);
		i++;
	}
	return (env_list);
}
// stock list_env to array
char	**env_list_to_array(t_env *lst)
{
	int	count = 0;
	t_env	*current;

	current = lst;
	while (current)
	{
		count++;
		current = current->next;
	}
	char	**env_array = malloc(sizeof(char *) * (count + 1));
	if (!env_array)
	{
		perror("Failed to allocate memory");
		return (NULL);
	}
	//copier les valeurs dans le tableau
	current = lst;
	int	i = 0;
	while (i < count)
	{
		env_array[i] = ft_strdup(current->data);
		if (!env_array)
		{
			int	j = 0;
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

