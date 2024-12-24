/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_export.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tambinin <tambinin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 10:19:55 by tambinin          #+#    #+#             */
/*   Updated: 2024/12/21 14:14:03 by tambinin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_valid_id(char *args)
{
	int	i;

	i = 1;
	if (!args || (!ft_isalpha(args[0]) && args[0] != '_'))
		return (0);
	while (args[i])
	{
		if (!ft_isalnum(args[i]) && args[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

int	not_a_valid_id(char *args)
{
	if (!is_valid_id(args))
	{
		set_st(1);
		ft_putstr_fd("export: '", STDERR_FILENO);
		ft_putstr_fd(args, STDERR_FILENO);
		ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
		return (0);
	}
	return (1);
}

void	display_export(t_env *env)
{
	t_env	*current;
	char	*equal_pos;

	current = env;
	while (current)
	{
		equal_pos = ft_strchr(current->data, '=');
		if (equal_pos)
		{
			printf("declare -x %.*s", (int)(equal_pos - current->data),
				current->data);
			printf("=\"%s\"\n", equal_pos + 1);
		}
		else
			printf("declare -x %s\n", current->data);
		current = current->next;
	}
	set_st(0);
}

void	export_loop(char **args, t_env **env)
{
	int		i;
	char	*equals_pos;
	char	*name;

	i = 0;
	while (args[++i])
	{
		equals_pos = ft_strchr(args[i], '=');
		if (!equals_pos)
		{
			if (!not_a_valid_id(args[i]))
				return ;
			else
				update_or_add_variable(env, args[i]);
		}
		else
		{
			name = ft_substr(args[i], 0, equals_pos - args[i]);
			if (!not_a_valid_id(name))
				return ;
			else
				update_or_add_variable(env, args[i]);
			free(name);
		}
	}
}

void	ft_export(char **args, t_env **env)
{
	if (!args[1])
	{
		display_export(*env);
		return ;
	}
	else
		export_loop(args, env);
}
