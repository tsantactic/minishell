/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_unset.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tambinin <tambinin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 10:22:14 by tambinin          #+#    #+#             */
/*   Updated: 2024/12/21 10:11:57 by tambinin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	unset_env_var(const char *var_name, t_env **env)
{
	t_env	*current;
	t_env	*prev;
	size_t	len;

	current = *env;
	prev = NULL;
	len = ft_strlen(var_name);
	while (current)
	{
		if (ft_strncmp(current->data, var_name, len) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				*env = current->next;
			free(current->data);
			free(current);
			set_st(0);
			return ;
		}
		prev = current;
		current = current->next;
	}
	set_st(0);
}

void	ft_unset(char **args, t_env **env)
{
	int	i;

	i = 1;
	if (!args[1])
	{
		set_st(0);
		return ;
	}
	while (args[i])
	{
		if (!is_valid_id(args[i]))
		{
			set_st(1);
			ft_putstr_fd("unset: '", STDERR_FILENO);
			ft_putstr_fd(args[i], STDERR_FILENO);
			ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
		}
		else
			unset_env_var(args[i], env);
		i++;
	}
}
