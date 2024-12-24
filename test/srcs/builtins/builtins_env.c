/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_env.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tambinin <tambinin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 10:22:27 by tambinin          #+#    #+#             */
/*   Updated: 2024/12/20 16:22:16 by tambinin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_env(char **argv, t_env **env)
{
	t_env	*curr;

	curr = NULL;
	if (argv && argv[1])
	{
		set_st(1);
		ft_putendl_fd("no arguments are allowed", STDERR_FILENO);
		return ;
	}
	if (!env || !*env)
	{
		set_st(1);
		ft_putendl_fd("env : environment is empty or not initialized",
			STDERR_FILENO);
		return ;
	}
	curr = *env;
	while (curr)
	{
		if (ft_strchr(curr->data, '=') != 0)
			ft_putendl_fd(curr->data, STDOUT_FILENO);
		curr = curr->next;
	}
	set_st(0);
}
