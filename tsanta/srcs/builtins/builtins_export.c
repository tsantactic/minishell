/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_export.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tambinin <tambinin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 10:19:55 by tambinin          #+#    #+#             */
/*   Updated: 2024/11/29 16:45:07 by tambinin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"


// check si arg == alpha_numeric
int	is_valid_id(char *args)
{
	int	i = 1;

    if (!args || (!ft_isalpha(args[0]) && args[0] != '_'))
		return (0);
	while (args[i])
	{
		if (!ft_isalnum(args[i]) && args[i] != '_')
			return (0);
		i++;
	}
	return (1);
}
// Ajoute ou met à jour une variable dans la liste chaînée
void update_or_add_variable(t_env **env, const char *var)
{
    size_t name_len;
    t_env *current;
    char *equals_pos;

    equals_pos = ft_strchr(var, '=');
    if (!equals_pos) // Pas de "=", rien à faire
        return;
    name_len = equals_pos - var; // Longueur de la partie nom
    current = *env;

    // Parcours pour trouver une variable existante
    while (current)
    {
        if (ft_strncmp(current->data, var, name_len) == 0 && current->data[name_len] == '=')
        {
            free(current->data);           // Supprime l'entrée existante
            current->data = ft_strdup(var); // Copie la nouvelle valeur
            return;
        }
        current = current->next;
    }

    // Si non trouvé, ajouter comme nouvelle variable
    add_env_node(env, (char *)var);
}

void ft_export(char **args, t_env **env)
{
    int i = 1;

    if (!args[1]) // Pas d'arguments : afficher l'environnement
    {
        t_env *current = *env;
        while (current)
        {
            printf("declare -x %s\n", current->data);
            current = current->next;
        }
        return;
    }

    while (args[i])
    {
        char *equals_pos = strchr(args[i], '=');
        if (!equals_pos) // Pas de "=", juste un identifiant
        {
            if (!is_valid_id(args[i]))
                fprintf(stderr, "export: '%s': not a valid identifier\n", args[i]);
            else
                update_or_add_variable(env, args[i]);
        }
        else // Contient "="
        {
            char *name = ft_substr(args[i], 0, equals_pos - args[i]);
            if (!is_valid_id(name))
                fprintf(stderr, "export: '%s': not a valid identifier\n", args[i]);
            else
                update_or_add_variable(env, args[i]);
            free(name);
        }
        i++;
    }
}
