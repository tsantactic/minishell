/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sandriam <sandriam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 10:43:42 by sandriam          #+#    #+#             */
/*   Updated: 2024/12/05 10:59:48 by sandriam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void parse_exec_redirection(t_cmd *cmd)
{
    if (contains_redirection(cmd))
    {
        if (redirection_exec(cmd) == 2)
        {
            if (contains_heredoc(cmd))
                free_pipe_heredoc(cmd->count_heredoc, cmd->pipe_heredoc);
            ft_free_token_cmd(cmd);
            free_tokens(cmd);
            free_new_env(cmd->env_lst);
            exit (1);
        }
    }
}

int redirection_exec(t_cmd *cmd)
{
    int k = 0;
    while (k < cmd->len_tokens)
    {
        if (cmd->tokens[k]->type == REDIR_OUT)
        {
            int fd = open(cmd->tokens[k + 1]->value, O_WRONLY | O_CREAT | O_TRUNC, 0777);
            if (fd < 0)
            {
				set_st(1);
                perror(cmd->tokens[k + 1]->value);
                return (1);
            }
            dup2(fd, 1);
            close(fd);
            k++;
        }
        if (cmd->tokens[k]->type == REDIR_IN)
        {
            int fd = open(cmd->tokens[k + 1]->value, O_RDONLY);
            if (fd < 0)
            {
				set_st(1);
                perror(cmd->tokens[k + 1]->value);                
                return (2);
            }
            int i = k + 1;
            int is_last_stdin = 0;
            while (i < cmd->len_tokens)
            {
                if (cmd->tokens[i]->type == REDIR_HEREDOC || cmd->tokens[i]->type == REDIR_IN)
                    is_last_stdin = 1;
                i++;
            }
            if (is_last_stdin == 0)
                dup2(fd, 0);
            close(fd);
            k++;
        }
        if (cmd->tokens[k]->type == REDIR_OUTPUT_APPEND)
        {
            int fd = open(cmd->tokens[k + 1]->value, O_WRONLY | O_APPEND | O_CREAT, 0777);
            if (fd < 0)
            {
				set_st(1);
                perror(cmd->tokens[k + 1]->value);
                return (1);
            }
            dup2(fd, 1);
            close(fd);
            k++;
        }
        k++;
    }
    return (0);
}

int contains_redirection(t_cmd *cmd)
{
    int i = 0;
    while (i < cmd->len_arg)
    {
        if (cmd->tokens[i]->type == REDIR_IN)
            return (1);
        if (cmd->tokens[i]->type == REDIR_OUT)
            return (1);
        if (cmd->tokens[i]->type == REDIR_OUTPUT_APPEND)
            return (1);
        i++;
    }
    return (0);
}
