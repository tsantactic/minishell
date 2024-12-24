/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executions_utils_supp.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tambinin <tambinin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 11:40:16 by sandriam          #+#    #+#             */
/*   Updated: 2024/12/20 13:47:19 by tambinin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	count_env(char *value)
{
	char	**split_env;
	int		count;

	split_env = ft_split(value, ' ');
	count = 0;
	while (split_env && split_env[count])
	{
		free(split_env[count]);
		count++;
	}
	free(split_env);
	return (count);
}

void	count_command_arg(t_cmd *cmd, int *len_command)
{
	int	i;

	i = 0;
	while (i < cmd->len_tokens)
	{
		if (cmd->tokens[i]->type == CMD || cmd->tokens[i]->type == ARG)
		{
			if (cmd->tokens[i]->type_env == IS_ENV
				&& ft_strchr(cmd->tokens[i]->value, ' ') != 0)
			{
				(*len_command) += count_env(cmd->tokens[i]->value);
			}
			else
				(*len_command)++;
		}
		i++;
	}
}

char	*copy_command_arg_blt(t_cmd *cmd, char **my_t_cmd, char *command)
{
	int	j;
	int	k;

	j = 0;
	k = 0;
	while (k < cmd->len_tokens)
	{
		if (cmd->tokens[k]->type == ARG || cmd->tokens[k]->type == CMD)
		{
			if (cmd->tokens[k]->type == CMD)
				command = ft_strdup(cmd->tokens[k]->value);
			my_t_cmd[j++] = ft_strdup(cmd->tokens[k]->value);
		}
		k++;
	}
	my_t_cmd[j] = NULL;
	return (command);
}

char	*add_value_env(char *value, int *j, char **my_t_cmd, char *command)
{
	char	**split_env;
	int		i;

	split_env = ft_split(value, ' ');
	i = 0;
	while (split_env && split_env[i])
	{
		my_t_cmd[(*j)++] = ft_strdup(split_env[i]);
		if (i == 0)
			command = ft_strdup(split_env[0]);
		free(split_env[i]);
		i++;
	}
	free(split_env);
	return (command);
}

char	*copy_command_arg(t_cmd *cmd, char **my_t_cmd, char *command)
{
	int	j;
	int	k;

	j = 0;
	k = 0;
	while (k < cmd->len_tokens)
	{
		if (cmd->tokens[k]->type == ARG || cmd->tokens[k]->type == CMD)
		{
			if (cmd->tokens[k]->type_env == IS_ENV
				&& ft_strchr(cmd->tokens[k]->value, ' ') != 0)
				command = add_value_env(cmd->tokens[k]->value, &j, my_t_cmd,
						command);
			else
			{
				if (cmd->tokens[k]->type == CMD)
					command = ft_strdup(cmd->tokens[k]->value);
				my_t_cmd[j++] = ft_strdup(cmd->tokens[k]->value);
			}
		}
		k++;
	}
	my_t_cmd[j] = NULL;
	return (command);
}
