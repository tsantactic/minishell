/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_exit.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tambinin <tambinin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 10:19:58 by tambinin          #+#    #+#             */
/*   Updated: 2024/11/21 11:04:21 by tambinin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void ft_exit(char **args)
{
    int exit_code = 0;
    int i = 0;

    rl_clear_history();
    ft_putendl_fd("exit", 1);
    if (args[1] == NULL)
        exit(0);
    else
    {
        while (args[1][i])
        {
            if (args[1][i] < '0' || args[1][i] > '9')
            {
                ft_putendl_fd("exit: numeric argument required", 2);
                exit(255);
            }
            i++;
        }
        exit_code = ft_atoi(args[1]);
        if (args[2] != NULL)
        {
            ft_putendl_fd("exit: too many arguments", 2);
            return ;
        }
    }
    exit(exit_code);
}
