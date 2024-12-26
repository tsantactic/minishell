/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_stdin_stdout.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sandriam <sandriam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 14:23:59 by tambinin          #+#    #+#             */
/*   Updated: 2024/12/24 15:49:13 by sandriam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	add_stdin(t_token **token, int len)
{
	int	i;
	int	set;
	int	is_redir_in;

	i = 0;
	set = 0;
	is_redir_in = 0;
	while (i < len)
	{
		if ((token[i]->type == CMD || token[i]->type == REDIR_OUT)
			&& !is_redir_in)
			set = 1;
		if (token[i]->type == REDIR_IN || token[i]->type == REDIR_HEREDOC)
		{
			is_redir_in = 1;
			set = 0;
		}
		i++;
	}
	return (set);
}

void	check_stdin(t_cmd *cmd)
{
	int	num_pipes;
	int	i;

	num_pipes = cmd->nb_pipe;
	i = 0;
	while (i < num_pipes)
	{
		cmd->tp_arg[i][0]->stdin_set = 0;
		if (add_stdin(cmd->tp_arg[i],
				cmd->tp_arg[i][0]->total_len) == 1)
			cmd->tp_arg[i][0]->stdin_set = 1;
		i++;
	}
	if (add_stdin(cmd->tp_arg[num_pipes],
			cmd->tp_arg[num_pipes][0]->total_len))
		cmd->tp_arg[num_pipes][0]->stdin_set = 1;
	else
		cmd->tp_arg[num_pipes][0]->stdin_set = 0;
}

int	add_stdout(t_token **token, int len)
{
	int	i;
	int	is_redir_out;
	int	is_cmd;

	i = 0;
	is_redir_out = 0;
	is_cmd = 0;
	while (i < len)
	{
		if (token[i]->type == CMD)
			is_cmd = 1;
		if (token[i]->type == REDIR_OUT)
			is_redir_out = 1;
		i++;
	}
	if (is_redir_out == 1 && is_cmd == 1)
		return (1);
	return (0);
}

void	check_stdout(t_cmd *cmd)
{
	int	num_pipes;
	int	i;

	num_pipes = cmd->nb_pipe;
	i = 0;
	while (i < num_pipes)
	{
		cmd->tp_arg[i][0]->stdout_set = 0;
		if (add_stdout(cmd->tp_arg[i],
				cmd->tp_arg[i][0]->total_len) == 1)
			cmd->tp_arg[i][0]->stdout_set = 1;
		i++;
	}
	if (add_stdout(cmd->tp_arg[num_pipes],
			cmd->tp_arg[num_pipes][0]->total_len) == 1)
		cmd->tp_arg[num_pipes][0]->stdout_set = 1;
	else
		cmd->tp_arg[num_pipes][0]->stdout_set = 0;
}
