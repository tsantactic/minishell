/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_with_pipe.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tambinin <tambinin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 11:23:13 by tambinin          #+#    #+#             */
/*   Updated: 2024/12/17 14:35:51 by tambinin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int parse_exec_redir_pipe(t_token **commands_arg, int len)
{
    if (contains_redir_pipe(commands_arg, len))
    {
        if (redirection_exec_pipe(commands_arg, len) == 2)
        {
            // exit (1);
            return (1);
        }
    }
    return (0);
}

int contains_redir_pipe(t_token **token, int len)
{
    int k = 0;
    
    while (k < len)
    {
        if (token[k]->type == REDIR_IN)
            return (1);
        if (token[k]->type == REDIR_OUT)
            return (1);
        if (token[k]->type == REDIR_OUTPUT_APPEND)
            return (1);
        k++;
    }
    return (0);
}

int redirection_exec_pipe(t_token **tokens, int len)
{
    int k = 0;
    while (k < len)
    {
        if (tokens[k]->type == REDIR_OUT)
        {
            int fd = open(tokens[k + 1]->value, O_WRONLY | O_CREAT | O_TRUNC, 0777);
            if (fd < 0)
            {
				set_st(1);
                perror(tokens[k + 1]->value);
                return (2);
            }
            dup2(fd, 1);
            close(fd);
            k++;
        }
        if (tokens[k]->type == REDIR_IN)
        {
            int fd = open(tokens[k + 1]->value, O_RDONLY);
            if (fd < 0)
            {
				set_st(1);
                perror(tokens[k + 1]->value);                
                return (2);
            }
            int i = k + 1;
            int is_last_stdin = 0;
            while (i < len)
            {
                if (tokens[i]->type == REDIR_HEREDOC || tokens[i]->type == REDIR_IN)
                    is_last_stdin = 1;
                i++;
            }
            if (is_last_stdin == 0)
                dup2(fd, 0);
            close(fd);
            k++;
        }
        if (tokens[k]->type == REDIR_OUTPUT_APPEND)
        {
            int fd = open(tokens[k + 1]->value, O_WRONLY | O_APPEND | O_CREAT, 0777);
            if (fd < 0)
            {
				set_st(1);
                perror(tokens[k + 1]->value);
                return (2);
            }
            dup2(fd, 1);
            close(fd);
            k++;
        }
        k++;
    }
    return (0);
}


