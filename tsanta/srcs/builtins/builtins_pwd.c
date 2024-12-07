/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_pwd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tambinin <tambinin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 10:21:58 by tambinin          #+#    #+#             */
/*   Updated: 2024/11/29 14:19:09 by tambinin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void    ft_pwd(char **argv)
{
    // getcwd : pour obtenir le chemin absolue du repertoire
    char cwd[1024];

    if (argv[1] != NULL)
    {
        ft_putendl_fd("no arguments are allowed", STDERR_FILENO);
        return;
    }
    if (getcwd(cwd, sizeof(cwd)) != NULL)
        ft_putendl_fd(cwd, STDOUT_FILENO);
    else
        perror("pwd");
}
