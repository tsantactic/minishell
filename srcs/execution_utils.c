/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sandriam <sandriam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 14:44:02 by sandriam          #+#    #+#             */
/*   Updated: 2024/11/18 14:20:02 by sandriam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*ft_is_path(char **envp)
{
	int		i;
	char	*my_path;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			my_path = envp[i] + 5;
			return (my_path);
		}
		i++;
	}
	return (NULL);
}
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
char	*ft_find_path(char *t_cmd, char **envp)
{
	int		j;
	char	**path;
	char	*path_t_cmd;

	path = ft_split(ft_is_path(envp), ':');
	if (!path)
		return (NULL);
	j = 0;
	while (path[j])
	{
		path_t_cmd = build_path_t_cmd(path[j], t_cmd);
		if (path_t_cmd && access(path_t_cmd, R_OK | F_OK | X_OK) == 0)
		{
			if (t_cmd[0] == '/' || t_cmd[0] == '.')
			{
				ft_free(path);
				return (NULL);
			}
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
void	ft_execute_command(t_cmd *cmd, char *path, char **my_t_cmd, char **envp)
{
	if (!path)
	{
		struct stat path_stat;
		if (stat(my_t_cmd[0], &path_stat) == 0)
		{
			if (S_ISDIR(path_stat.st_mode))
			{
				int i = 0;
				int is_not_cmd= 0;
				int is_not_dir = 0;
				while (my_t_cmd[i])
				{
					if (ft_strchr(my_t_cmd[i], '.'))
					{
						is_not_cmd++;
						if (ft_strcmp(my_t_cmd[i], "..") == 0)
						{
							is_not_cmd++;
						}
					}
					if (ft_strchr(my_t_cmd[i], '/'))
					{
						is_not_dir++;
					}
					i++;
				}
				ft_putstr_fd("minishell: ", 2);
				ft_putstr_fd(my_t_cmd[0], 2);
				if (is_not_cmd == 1 && !is_not_dir)
				{
					ft_putstr_fd(": filename argument required\n.: usage: . filename [arguments]\n", 2);	
				}
				else if (is_not_cmd == 2 && !is_not_dir)
				{
					ft_putstr_fd(":command not found\n", 2);
				}
				else
					ft_putstr_fd(": Is a directory\n", 2);
			}
		}
		else
		{
			if(errno == 20)
			{
				ft_putstr_fd("bash: ", 2);
				ft_putstr_fd(my_t_cmd[0], 2);
				ft_putstr_fd(": Not a directory\n", 2);
			}
			else if (errno == 2)
			{
				int i = 0;
				int is_not_cmd = 0;
				while (my_t_cmd[i])
				{
					if (ft_strchr(my_t_cmd[i], '/'))
					{
						is_not_cmd = 1;
						break;
					}
					i++;
				}
				ft_putstr_fd("minishell: ", 2);
				ft_putstr_fd(my_t_cmd[0], 2);
				if (is_not_cmd)
					ft_putstr_fd(": No such file or directory\n", 2);
				else
					ft_putstr_fd(": command not found\n", 2);
			}
		}
		ft_free(my_t_cmd);
		free(path);
		free_tokens_and_args(cmd);
		exit(1);
	}
	else if (execve(path, my_t_cmd, envp) == -1)
	{
		struct stat path_stat;
		if (stat(my_t_cmd[0], &path_stat) == 0)
		{
			if (S_ISDIR(path_stat.st_mode))
			{
				ft_putstr_fd("minishell: ", 2);
				ft_putstr_fd(my_t_cmd[0], 2);
				ft_putstr_fd(": Is a directory\n", 2);
			}
		}
		free_token_list(cmd);
		ft_free(my_t_cmd);
		free(path);
		ft_free(cmd->args);
	}
}
