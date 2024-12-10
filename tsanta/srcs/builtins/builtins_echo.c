/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_echo.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sandriam <sandriam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 12:58:19 by tambinin          #+#    #+#             */
/*   Updated: 2024/12/04 17:33:51 by sandriam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void    ft_echo(char **args, t_env **env)
{
    int new_line = 1; // echo sans "-n" -> saut a la ligne
    int i = 1;

    while (args[i] && args[i][0] == '-' && args[i][1] == 'n')
    {
        int j = 1;
        while (args[i][j] == 'n')
            j++;
        if (args[i][j] == '\0')
        {
            new_line = 0; //pas de saut a la ligne
            i++;
        }
        else
            break ;
    }
    //parcourt les arg restants
    while (args[i])
    {
        // if (args[i][0] == '$')
        // {
        //     const char  *value = get_env_value(args[i] + 1, env);
        //     ft_putstr_fd((char *)value, STDOUT_FILENO);
        // }
        // else
        (void)env;
        ft_putstr_fd(args[i], STDOUT_FILENO);
		if (args[i + 1])
			write(1, " ", 1);
		i++;
    }
    if (new_line)
		write(1, "\n", 1);
    set_st(0);
    return ;
}
