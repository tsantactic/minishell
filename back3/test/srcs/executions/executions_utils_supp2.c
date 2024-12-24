/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executions_utils_supp2.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tambinin <tambinin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 15:36:42 by tambinin          #+#    #+#             */
/*   Updated: 2024/12/21 15:46:12 by tambinin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	contains_bin(char *command)
{
	if ((ft_strncmp(command, "/bin/", 5) == 0 || ft_strncmp(command,
				"/usr/bin/", 9) == 0))
		return (1);
	return (0);
}

void	count_command_arg_blt(t_cmd *cmd, int *len_command)
{
	int	i;

	i = 0;
	while (i < cmd->len_tokens)
	{
		if (cmd->tokens[i]->type == CMD || cmd->tokens[i]->type == ARG)
			(*len_command)++;
		i++;
	}
}

char	*copy_command(t_cmd *cmd, char *command)
{
	int	k;

	k = 0;
	while (k < cmd->len_tokens)
	{
		if (cmd->tokens[k]->type == ARG || cmd->tokens[k]->type == CMD)
		{
			if (cmd->tokens[k]->type == CMD)
				command = ft_strdup(cmd->tokens[k]->value);
		}
		k++;
	}
	return (command);
}

char	*extract_command_bin(char *command)
{
	if (contains_bin(command))
	{
		if (ft_strncmp(command, "/bin/", 5) == 0)
			return (command + 5);
		else if (ft_strncmp(command, "/usr/bin/", 9) == 0)
			return (command + 9);
	}
	return (ft_strdup(command));
}

int	check_env(t_env **envp, char *value)
{
	t_env	*curr;

	curr = NULL;
	curr = *envp;
	while (curr)
	{
		if (ft_strncmp(curr->data, value, ft_strlen(value)) == 0)
			return (1);
		curr = curr->next;
	}
	return (0);
}
