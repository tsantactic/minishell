/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_arg_builtins_utils.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sandriam <sandriam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 10:27:06 by tambinin          #+#    #+#             */
/*   Updated: 2024/12/24 11:33:30 by sandriam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	init_parsing(t_cmd *cmd)
{
	int	temp_pipe_heredoc;

	temp_pipe_heredoc = 1;
	cmd->pipe_heredoc = NULL;
	cmd->count_heredoc = 0;
	cmd->stdout_backup = dup(STDOUT_FILENO);
	cmd->stdin_backup = dup(STDIN_FILENO);
	exec_heredoc_builtins(cmd, temp_pipe_heredoc);
}

void	clean_memory(t_cmd *cmd, char **tmp_cmd)
{
	if (contains_heredoc(cmd))
		free_pipe_heredoc(cmd->count_heredoc, cmd->pipe_heredoc);
	free_tokens(cmd);
	if (tmp_cmd)
		ft_free(tmp_cmd);
	set_st(0);
	return ;
}

void	reset_after_builtin(t_cmd *cmd, char **tmp_cmd)
{
	dup2(cmd->stdout_backup, STDOUT_FILENO);
	dup2(cmd->stdin_backup, STDIN_FILENO);
	close(cmd->stdout_backup);
	close(cmd->stdin_backup);
	if (tmp_cmd)
		ft_free(tmp_cmd);
	if (contains_heredoc(cmd))
		free_pipe_heredoc(cmd->count_heredoc, cmd->pipe_heredoc);
}
