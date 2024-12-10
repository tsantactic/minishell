/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_pwd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sandriam <sandriam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 10:21:58 by tambinin          #+#    #+#             */
/*   Updated: 2024/12/06 16:48:56 by sandriam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void    ft_pwd(char **argv)
{
    // getcwd : pour obtenir le chemin absolue du repertoire
    char cwd[1024];

    if (argv[1] != NULL)
    {
        set_st(1);
        ft_putendl_fd("no arguments are allowed", STDERR_FILENO);
        return;
    }
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        set_st(0);
        ft_putendl_fd(cwd, STDOUT_FILENO);
    }
    else
        perror("pwd");
}
