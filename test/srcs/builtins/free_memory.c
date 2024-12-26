/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_memory.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sandriam <sandriam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 13:52:46 by sandriam          #+#    #+#             */
/*   Updated: 2024/12/24 15:49:13 by sandriam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	free_exit_simple(char **args, t_cmd *cmd, t_env **env, long exit_code)
{
	exit_code = ft_atol(args[1]);
	set_st(exit_code);
	dup2(cmd->stdout_backup, STDOUT_FILENO);
	dup2(cmd->stdin_backup, STDIN_FILENO);
	close(cmd->stdout_backup);
	close(cmd->stdin_backup);
	if (args)
		ft_free(args);
	if (contains_heredoc(cmd))
		free_pipe_heredoc(cmd->count_heredoc, cmd->pipe_heredoc);
	ft_free_token_cmd(cmd);
	free_new_env(env);
	free_tokens(cmd);
	exit(exit_code);
}

void	free_exit_no_arg(t_cmd *cmd, char **args, t_env **env)
{
	dup2(cmd->stdout_backup, STDOUT_FILENO);
	dup2(cmd->stdin_backup, STDIN_FILENO);
	close(cmd->stdout_backup);
	close(cmd->stdin_backup);
	if (args)
		ft_free(args);
	if (contains_heredoc(cmd))
		free_pipe_heredoc(cmd->count_heredoc, cmd->pipe_heredoc);
	ft_free_token_cmd(cmd);
	free_new_env(env);
	free_tokens(cmd);
}

void	free_commands_exit(t_cmd *cmd)
{
	int	i;
	int	j;

	i = 0;
	while (i <= cmd->nb_pipe)
	{
		ft_free(cmd->tp_arg[i][0]->tp_args);
		j = 0;
		while (cmd->tp_arg[i][j] && cmd->tp_arg[i][j]->value)
		{
			if (cmd->tp_arg[i][j]->type == CMD && cmd->index_exit != i)
				free(cmd->tp_arg[i][0]->tp_val);
			free(cmd->tp_arg[i][j]);
			j++;
		}
		if (cmd->tp_arg[i])
			free(cmd->tp_arg[i]);
		i++;
	}
}

void	free_exit_pipe(t_cmd *cmd, t_env **env)
{
	int	k;

	k = 0;
	while (k < cmd->nb_pipe)
		free(cmd->fd_pipe[k++]);
	free(cmd->fd_pipe);
	free(cmd->pid);
	free_commands_exit(cmd);
	free(cmd->tp_arg);
	ft_free_token_cmd(cmd);
	free_tokens(cmd);
	free_new_env(env);
}
