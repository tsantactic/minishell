/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_echo.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tambinin <tambinin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 12:58:19 by tambinin          #+#    #+#             */
/*   Updated: 2024/12/21 11:02:31 by tambinin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	build_echo(char **args, int *new_line)
{
	int	i;
	int	j;

	i = 1;
	while (args[i] && args[i][0] == '-' && args[i][1] == 'n')
	{
		j = 1;
		while (args[i][j] == 'n')
			j++;
		if (args[i][j] == '\0')
		{
			(*new_line) = 0;
			i++;
		}
		else
			break ;
	}
	while (args[i])
	{
		ft_putstr_fd(args[i], STDOUT_FILENO);
		if (args[i + 1])
			write(1, " ", 1);
		i++;
	}
}

void	ft_echo(char **args)
{
	int	new_line;

	new_line = 1;
	build_echo(args, &new_line);
	if (new_line)
		write(1, "\n", 1);
	set_st(0);
	return ;
}
