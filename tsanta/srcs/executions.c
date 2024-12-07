/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sandriam <sandriam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 10:06:14 by sandriam          #+#    #+#             */
/*   Updated: 2024/12/04 13:48:13 by sandriam         ###   ########.fr       */
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
void loop_simple_heredoc(char *delimiter, t_cmd *cmd)
{
    char *input = NULL;
	set_sig_heredoc(0);
	cmd->stdin_heredoc = dup(STDIN_FILENO);
    while (1)
    {
		signal(SIGINT, sig_handler_heredoc_blt);
        input = readline("heredoc>");
		if (set_sig_heredoc(-1) == 1)
        {
			if (input)
                free(input);
            break;
        }
        if (!input)
        {
			set_sig_new_line(1);
            ft_putstr_fd("minishel : warning: here-document at this line delimited by end-of-file (wanted :'",2);
            ft_putstr_fd(delimiter, 2);
            ft_putstr_fd("')\n", 2);
            set_st(0);
            break;
        }
        if (ft_strcmp(input, delimiter) == 0)
        {
            free(input);
            break;
        }
        free(input);
    }
	dup2(cmd->stdin_heredoc, 0);
    close(cmd->stdin_heredoc);
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
				loop_simple_heredoc(cmd->tokens[i+1]->value, cmd);
			}
		}
		i++;
	}
	if (tmp == -2 && set_sig_heredoc(-1) == 0)
		ft_putendl_fd("minishell: syntax error near unexpected token `newline'",2);
	if (set_sig_heredoc(-1) == 1)
	{
		set_sig_new_line(1);
		set_sig_heredoc(0);
	}
}

void exec_alls(t_cmd *cmd, t_env **env)
{
    count_pipe(cmd);
	int tmp = 0;
	set_sig_new_line(0);
	tmp = check_error_arg(cmd->tokens, cmd->len_tokens);
    if (cmd->nb_pipe == 0 && cmd->len_arg > 0)
	{
		if (tmp > 0)
        	execute_without_pipe(cmd, env); /*****  execution without pipe simple command****/
		else
		{
			execute_possibility_heredoc(cmd, tmp);
			return ;
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
