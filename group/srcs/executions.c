/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sandriam <sandriam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 10:06:14 by sandriam          #+#    #+#             */
/*   Updated: 2024/11/28 16:23:18 by sandriam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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
void loop_simple_heredoc(char *delimiter)
{
    char *input = NULL;
    while (1)
    {
        input = readline("heredoc>");
        if (!input)
            break;
        if (ft_strcmp(input, delimiter) == 0)
        {
            free(input);
            break;
        }
        free(input);
    }
}

void execute_possibility_heredoc(t_cmd *cmd, int tmp)
{
	int i = 0;
	while (i < set_index_syntax(cmd->tokens, cmd->len_tokens))
	{
		if (cmd->tokens[i]->type == REDIR_HEREDOC)
		{
			if (cmd->tokens[i+1]->value)
			{
				loop_simple_heredoc(cmd->tokens[i+1]->value);
			}
		}
		i++;
	}
	if (tmp == -2)
		ft_putendl_fd("minishell: syntax error near unexpected token `newline'",2);
}
void exec_alls(t_cmd *cmd, char **env)
{
    count_pipe(cmd);
	int tmp = 0;
	tmp = check_error_arg(cmd->tokens, cmd->len_tokens);
    if (cmd->nb_pipe == 0 && cmd->len_arg > 0)
	{
		if (tmp > 0)
        	execute_without_pipe(cmd, env); /*****  execution without pipe simple command****/
		else
		{
			execute_possibility_heredoc(cmd, tmp);
		}
	}
    else if (cmd->nb_pipe > 0 && cmd->len_arg > 0)
	{
		if (tmp > 0)
			execute_with_pipes(cmd, env); /*** execution with pipe *****/
		else
		{
			execute_possibility_heredoc(cmd, tmp);
		}
	}
}
