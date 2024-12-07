/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_env.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tambinin <tambinin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 10:22:27 by tambinin          #+#    #+#             */
/*   Updated: 2024/11/29 11:38:18 by tambinin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <unistd.h>

void ft_env(char **argv, t_env **env)
{
	t_env *curr = NULL;

	if (argv && argv[1])
	{
		ft_putendl_fd("no arguments are allowed", STDERR_FILENO);
		return ;
	}
	if (!env || !*env)
	{
		ft_putendl_fd("env : environment is empty or not initialized", STDERR_FILENO);
		return ;
	}
	curr = *env;
	while (curr->next)
	{
		ft_putendl_fd(curr->data, STDOUT_FILENO);
		curr = curr->next;
	}
}
