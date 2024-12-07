/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_echo.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tambinin <tambinin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 12:58:19 by tambinin          #+#    #+#             */
/*   Updated: 2024/11/22 09:55:37 by tambinin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// N.B : gestion \/$ \\/$
void    ft_echo(char **args)
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
        char    *arg = args[i];

        ft_putstr_fd(arg, 1);
		if (args[i + 1])
			write(1, " ", 1);
		i++;
    }
    if (new_line)
		write(1, "\n", 1);
}
