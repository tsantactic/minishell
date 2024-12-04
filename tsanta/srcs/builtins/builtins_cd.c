/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_cd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tambinin <tambinin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 10:20:23 by tambinin          #+#    #+#             */
/*   Updated: 2024/11/29 17:16:47 by tambinin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void ft_cd(char **args, t_env **env)
{
    // char cwd[1024];
    const char *home = NULL;
    // const char *old_pwd = NULL;

    // Récupérer les valeurs des variables HOME et PWD
    home = get_env_value("HOME", env);
    // old_pwd = get_env_value("PWD", env);

    // Si aucun argument ou "cd ~"
    if (!args[1] || ft_strcmp(args[1], "~") == 0)
    {
        if (!home || *home == '\0')
        {
            ft_putendl_fd("cd: HOME not set", STDERR_FILENO);
            return;
        }
        if (chdir(home) != 0)
        {
            perror("cd");
            return;
        }
    }
    else if (chdir(args[1]) != 0) // Autres cas avec un argument
    {
        perror("cd");
        return;
    }

    // Mettre à jour PWD et OLDPWD
    // if (getcwd(cwd, sizeof(cwd)) != NULL)
    // {
    //     update_env_variable("OLDPWD", old_pwd ? old_pwd : "", env);
    //     update_env_variable("PWD", cwd, env);
    // }
    // else
    // {
    //     perror("getcwd");
    // }
}