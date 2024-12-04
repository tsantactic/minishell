/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tambinin <tambinin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 13:38:53 by tambinin          #+#    #+#             */
/*   Updated: 2024/11/29 16:34:49 by tambinin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// Met à jour une variable dans la liste d'environnement
void update_env_variable(const char *name, const char *value, t_env **env_list)
{
    t_env *current = *env_list;
    size_t name_len = ft_strlen(name);
    char *new_entry = NULL;

    // Crée une nouvelle chaîne "VAR=VALUE"
    new_entry = malloc(name_len + ft_strlen(value) + 2); // "name=value\0"
    if (!new_entry)
    {
        perror("malloc");
        return;
    }
    ft_strlcpy(new_entry, name, name_len + 1);
    ft_strlcat(new_entry, "=", name_len + 2);
    ft_strlcat(new_entry, value, name_len + ft_strlen(value) + 2);

    // Parcours pour trouver et remplacer la variable existante
    while (current)
    {
        if (ft_strncmp(current->data, name, name_len) == 0
                && current->data[name_len] == '=')
        {
            free(current->data);
            current->data = new_entry;
            return;
        }
        current = current->next;
    }

    // Si la variable n'existe pas, ajoutez-la
    t_env *new_node = malloc(sizeof(t_env));
    if (!new_node)
    {
        free(new_entry);
        perror("malloc");
        return;
    }
    new_node->data = new_entry;
    new_node->next = *env_list;
    *env_list = new_node;
}




