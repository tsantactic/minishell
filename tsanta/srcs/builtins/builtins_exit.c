/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_exit.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sandriam <sandriam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 10:19:58 by tambinin          #+#    #+#             */
/*   Updated: 2024/12/10 16:16:35 by sandriam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void ft_exit(char **args)
{
    int exit_code = 0;
    int i = 0;

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
                set_st(2);
                rl_clear_history();
                return ;
            }
            i++;
        }
        exit_code = ft_atoi(args[1]);
        if (args[2] != NULL)
        {
            set_st(1);
            ft_putendl_fd("exit: too many arguments", 2);
            return ;
        }
    }
    set_st(exit_code);
    rl_clear_history();
    return ;
}
