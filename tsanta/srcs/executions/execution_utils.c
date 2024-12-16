/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sandriam <sandriam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 14:44:02 by sandriam          #+#    #+#             */
/*   Updated: 2024/12/10 12:44:32 by sandriam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ft_perror(char *msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}

char	*build_path_t_cmd(char *path, char *t_cmd)
{
	char	*part_path;
	char	*path_t_cmd;

	part_path = ft_strjoin(path, "/");
	if (!part_path)
		return (NULL);
	path_t_cmd = ft_strjoin(part_path, t_cmd);
	free(part_path);
	return (path_t_cmd);
}
char	*ft_is_path(t_env **envp)
{
	char	*my_path;
  	t_env *curr = NULL;

	curr = *envp;
	while (curr)
	{
		if (ft_strncmp(curr->data, "PATH=", 5) == 0)
		{
			my_path = curr->data + 5;
			return (my_path);
		}
		curr = curr->next;
	}
	return (NULL);
}

char	*ft_find_path(char *t_cmd, t_env **envp)
{
	int		j;
	char	**path;
	char	*path_t_cmd;

	path = ft_split(ft_is_path(envp), ':');
	if (path == NULL)
		return (NULL);
	j = 0;
	while (path[j])
	{
		path_t_cmd = build_path_t_cmd(path[j], t_cmd);
		if (path_t_cmd && access(path_t_cmd, R_OK | F_OK | X_OK) == 0)
		{
			ft_free(path);
			return (path_t_cmd);
		}
		free(path_t_cmd);
		j++;
	}
	ft_free(path);
	return (NULL);
}

void free_tokens_and_args(t_cmd *cmd)
{
    int j = 0;
    while (j < cmd->len_arg)
	{
        free(cmd->token_arg[j]);
        j++;
    }
    free(cmd->token_arg);
    cmd->token_arg = NULL;
    j = 0;
    while (cmd->args[j] != NULL)
	{
        free(cmd->args[j++]);
    }
    free(cmd->args);
    free_tokens(cmd);
}
