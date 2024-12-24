/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tambinin <tambinin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 10:06:14 by sandriam          #+#    #+#             */
/*   Updated: 2024/12/22 13:34:18 by tambinin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	count_pipe(t_cmd *cmd)
{
	int	i;

	i = 0;
	cmd->nb_pipe = 0;
	while (i < cmd->len_tokens)
	{
		if (cmd->tokens[i]->type == 4)
			cmd->nb_pipe += 1;
		i++;
	}
}

void	execute_possibility_heredoc(t_cmd *cmd, int tmp)
{
	int	i;

	i = 0;
	while (i < set_index_syntax(cmd->tokens, cmd->len_tokens))
	{
		if (cmd->tokens[i]->type == REDIR_HEREDOC)
		{
			if (cmd->tokens[i + 1]->value)
				loop_simple_heredoc(cmd->tokens[i + 1]->value, cmd);
		}
		i++;
	}
	if (tmp == -2 && set_sig_heredoc(-1) == 0)
		ft_putendl_fd("minishell: syntax error near unexpected token `newline'",
			2);
	if (set_sig_heredoc(-1) == 1)
	{
		set_sig_new_line(1);
		set_sig_heredoc(0);
	}
}

void	exec_alls(t_cmd *cmd, t_env **env)
{
	int	tmp;

	tmp = 0;
	set_sig_new_line(0);
	tmp = check_error_arg(cmd->tokens, cmd->len_tokens);
	count_pipe(cmd);
	if (cmd->nb_pipe == 0 && cmd->len_arg > 0)
	{
		if (tmp > 0)
			execute_without_pipe(cmd, env);
		else
		{
			execute_possibility_heredoc(cmd, tmp);
			return ;
		}
	}
	else if (cmd->nb_pipe > 0 && cmd->len_arg > 0)
	{
		if (tmp > 0)
			execute_with_pipes(cmd, env);
		else
			execute_possibility_heredoc(cmd, tmp);
	}
}
