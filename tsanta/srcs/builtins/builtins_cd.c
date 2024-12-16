/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_cd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sandriam <sandriam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 10:20:23 by tambinin          #+#    #+#             */
/*   Updated: 2024/12/04 18:00:51 by sandriam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void ft_cd(char **args, t_env **env)
{
    const char *old_pwd;
    char cwd[1024];

    // Récupérer l'ancien PWD
    old_pwd = get_env_value("PWD", env);

    // Si aucun argument ou "~", aller dans le home directory
    if (!args[1] || ft_strcmp(args[1], "~") == 0)
    {
        const char *home = get_env_value("HOME", env);
        if (!home || chdir(home) != 0)
        {
            set_st(1);
            perror("cd");
            return;
        }
    }
    else if (chdir(args[1]) != 0) // Changer de répertoire
    {
        set_st(1);
        ft_putstr_fd("cd: ", 2);
        perror(args[1]);
        return;
    }
    set_st(0);
    // Mettre à jour PWD et OLDPWD
    if (old_pwd)
        update_env_var("OLDPWD", old_pwd, env);
    if (getcwd(cwd, sizeof(cwd)) != NULL)
        update_env_var("PWD", cwd, env);
    else
        perror("getcwd");
}
