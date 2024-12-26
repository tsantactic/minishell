/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_command_simple.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sandriam <sandriam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 11:00:30 by sandriam          #+#    #+#             */
/*   Updated: 2024/12/25 10:53:07 by sandriam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	free_values(char **tmp_cmd, t_cmd *cmd)
{
	if (tmp_cmd)
		ft_free(tmp_cmd);
	free_tokens(cmd);
}

void	parse_exec_command(t_cmd *cmd, t_env **env)
{
	char	**tmp_cmd;
	int		len_command;
	char	*command;

	len_command = 0;
	tmp_cmd = NULL;
	command = NULL;
	count_command_arg(cmd, &len_command);
	cmd->len_cmd = len_command;
	tmp_cmd = malloc(sizeof(char *) * (len_command + 1));
	if (!tmp_cmd)
	{
		free_tokens(cmd);
		return ;
	}
	command = copy_command_arg(cmd, tmp_cmd, command);
	if (command)
		handle_command_execution(cmd, command, tmp_cmd, env);
	else
	{
		cleanup_parsing(cmd, tmp_cmd);
		return ;
	}
	free_values(tmp_cmd, cmd);
}

void	parsing_argument_simple(t_cmd *cmd, t_env **env)
{
	cmd->env_lst = env;
	parse_exec_heredoc(cmd);
	if (set_sig_heredoc(-1) == 1)
	{
		set_sig_heredoc(0);
		free_tokens(cmd);
		free_pipe_heredoc(cmd->count_heredoc, cmd->pipe_heredoc);
		return ;
	}
	parse_exec_redirection(cmd);
	parse_exec_command(cmd, env);
	return ;
}
