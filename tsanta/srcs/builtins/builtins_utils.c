/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tambinin <tambinin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 13:38:53 by tambinin          #+#    #+#             */
/*   Updated: 2024/12/04 13:23:39 by tambinin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void update_env_var(const char *var_name, const char *value, t_env **env_list)
{
    size_t len = ft_strlen(var_name);
    t_env *current = *env_list;
    char *new_data;

    while (current)
    {
        if (ft_strncmp(current->data, var_name, len) == 0 && current->data[len] == '=')
        {
            free(current->data);
            new_data = ft_strjoin3(var_name, "=", value);
            current->data = new_data;
            return;
        }
        current = current->next;
    }
    // Si la variable n'existe pas -> l'ajouter
    new_data = ft_strjoin3(var_name, "=", value);
    add_env_node(env_list, new_data);
}

char *ft_strjoin3(const char *s1, const char *s2, const char *s3)
{
    char *result;
    size_t len = ft_strlen(s1) + ft_strlen(s2) + ft_strlen(s3);

    result = malloc(len + 1);
    if (!result)
        return (NULL);
    ft_strlcpy(result, s1, len + 1);
    ft_strlcat(result, s2, len + 1);
    ft_strlcat(result, s3, len + 1);
    return (result);
}




