/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_export.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tambinin <tambinin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 10:19:55 by tambinin          #+#    #+#             */
/*   Updated: 2024/12/13 16:42:10 by tambinin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"


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
// Ajoute ou met à jour une variable 
void update_or_add_variable(t_env **env, const char *var)
{
    size_t name_len;
    t_env *current;
    char *equal_pos;

    equal_pos = ft_strchr(var, '=');
    if (!equal_pos) // Pas de "=", rien à faire
       name_len = ft_strlen(var); 
    else
        name_len = equal_pos - var; // Longueur de la partie nom
    current = *env;
    // Parcours pour trouver une variable existante
    while (current) 
    {
        if (ft_strncmp(current->data, var, name_len) == 0 &&
            (current->data[name_len] == '=' || current->data[name_len] == '\0'))
        {
            if (equal_pos)
            {
                free(current->data);           // Supprime l'entrée existante
                current->data = ft_strdup(var); // Copie la nouvelle valeur
                set_st(0);
            }
            return;
        }
        current = current->next;
    }
    add_env_node(env, (char *)var);
    set_st(0);
}

void display_export(t_env *env)
{
    t_env *current = env;

    while (current)
    {
        char *equal_pos = ft_strchr(current->data, '=');
        if (equal_pos)
        {
            printf("declare -x %.*s", (int)(equal_pos - current->data), current->data); // avant le signe "=" -> VAR_NAME
            printf("=\"%s\"\n", equal_pos + 1); // apres le signe "=" avec /"/" -> "value"
        }
        else
            printf("declare -x %s\n", current->data); // Si pas de `=`, affiche uniquement le nom
        current = current->next;
    }
    set_st(0);
}

void ft_export(char **args, t_env **env)
{
    int i = 1;

    if (!args[1]) // Pas d'arguments : afficher l'environnement
    {
        display_export(*env);
        return;
    }

    while (args[i])
    {
        char *equals_pos = strchr(args[i], '=');
        if (!equals_pos) // Pas de "=", juste un identifiant
        {
            if (!is_valid_id(args[i]))
            {
                set_st(1);
                ft_putstr_fd("export: '", STDERR_FILENO);
                ft_putstr_fd(args[i], STDERR_FILENO);
                ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
            }
            else
                update_or_add_variable(env, args[i]);
        }
        else // Contient "="
        {
            char *name = ft_substr(args[i], 0, equals_pos - args[i]);
            if (!is_valid_id(name))
            {
                set_st(1);
                ft_putstr_fd("export: '", STDERR_FILENO);
                ft_putstr_fd(args[i], STDERR_FILENO);
                ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
            }
            else
                update_or_add_variable(env, args[i]);
            free(name);
        }
        i++;
    }
}
