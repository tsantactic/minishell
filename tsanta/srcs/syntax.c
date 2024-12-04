/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sandriam <sandriam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 10:38:31 by sandriam          #+#    #+#             */
/*   Updated: 2024/11/29 10:22:55 by sandriam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int is_redirection(t_token **tokens)
{
    if (tokens[0]->type == REDIR_IN || tokens[0]->type == REDIR_OUT || tokens[0]->type == REDIR_OUTPUT_APPEND || tokens[0]->type == REDIR_HEREDOC)
        return (1);
    return (0);    
}

int check_error_arg(t_token **tokens, int len_tokens)
{
    if (len_tokens == 1 && (is_redirection(tokens)))
    {
        set_st(2);
        printf("minishell: syntax error near unexpected token `newline'\n");
        return (-1);
    }
    else
    {
        int i = 0;
        while (i < len_tokens)
        {
            if ((tokens[i]->type == REDIR_IN || tokens[i]->type == REDIR_OUT || tokens[i]->type == REDIR_OUTPUT_APPEND || tokens[i]->type == REDIR_HEREDOC) && i + 1 == len_tokens)
            {
                set_st(2);
                return (-2);
            }
            if (tokens[i]->type == PIPE && i + 1 == len_tokens)
            {
                set_st(2);
                ft_putendl_fd("minishell: syntax error with unclosed '|'",2);
                return (-1);
            }
            if (tokens[i]->type == REDIR_OUT && tokens[i + 1]->type == PIPE)
            {
                set_st(2);
                ft_putendl_fd("minishell: syntax error near unexpected token `|'",2);
                return (-1);
            }
            if ((tokens[i]->type == REDIR_OUTPUT_APPEND || tokens[i]->type == REDIR_HEREDOC || tokens[i]->type == REDIR_OUT || tokens[i]->type == REDIR_IN) && tokens[i + 1]->type == REDIR_OUTPUT_APPEND)
            {
                set_st(2);
                ft_putendl_fd("minishell: syntax error near unexpected token '>>'",2);
                return (-1);
            }
            if ((tokens[i]->type == REDIR_OUTPUT_APPEND || tokens[i]->type == REDIR_HEREDOC || tokens[i]->type == REDIR_OUT || tokens[i]->type == REDIR_IN) && tokens[i + 1]->type == REDIR_HEREDOC)
            {
                set_st(2);
                ft_putendl_fd("minishell: syntax error near unexpected token '<<'",2);
                return (-1);
            }
            if ((tokens[i]->type == REDIR_OUTPUT_APPEND || tokens[i]->type == REDIR_HEREDOC || tokens[i]->type == REDIR_IN) && tokens[i + 1]->type == PIPE)
            {
                set_st(2);
                ft_putendl_fd("minishell: syntax error near unexpected token '|'",2);
                return (-1);
            }
            if ((tokens[i]->type == REDIR_OUTPUT_APPEND || tokens[i]->type == REDIR_HEREDOC || tokens[i]->type == REDIR_OUT || tokens[i]->type == REDIR_IN) && tokens[i + 1]->type == REDIR_OUT)
            {
                set_st(2);
                ft_putendl_fd("minishell: syntax error near unexpected token '>'",2);
                return (-1);
            }
            if ((tokens[i]->type == REDIR_OUTPUT_APPEND || tokens[i]->type == REDIR_HEREDOC || tokens[i]->type == REDIR_OUT || tokens[i]->type == REDIR_IN) && tokens[i + 1]->type == REDIR_IN)
            {
                set_st(2);
                ft_putendl_fd("minishell: syntax error near unexpected token '<'",2);
                return (-1);
            }
            if (tokens[i]->type == PIPE && tokens[i + 1]->type == PIPE)
            {
                set_st(2);
                ft_putendl_fd("minishell: syntax error near unexpected token '||'",2);
                return (-1);
            }
            i++;
        }
    }
    return (1);
}

int set_index_syntax(t_token **tokens, int len_tokens)
{
    int i = 0;
    while (i < len_tokens)
    {
        if ((tokens[i]->type == REDIR_IN || tokens[i]->type == REDIR_OUT || tokens[i]->type == REDIR_OUTPUT_APPEND || tokens[i]->type == REDIR_HEREDOC) && i + 1 == len_tokens)
        {
            return (i);
        }
        if (tokens[i]->type == PIPE && i + 1 == len_tokens)
        { 
            return (i);
        }
        if (tokens[i]->type == REDIR_OUT && tokens[i + 1]->type == PIPE)
        {
            return (i);
        }
        if ((tokens[i]->type == REDIR_OUTPUT_APPEND || tokens[i]->type == REDIR_HEREDOC || tokens[i]->type == REDIR_OUT || tokens[i]->type == REDIR_IN) && tokens[i + 1]->type == REDIR_OUTPUT_APPEND)
        {
            return (i);
        }
        if ((tokens[i]->type == REDIR_OUTPUT_APPEND || tokens[i]->type == REDIR_HEREDOC || tokens[i]->type == REDIR_OUT || tokens[i]->type == REDIR_IN) && tokens[i + 1]->type == REDIR_HEREDOC)
        {
            return (i);
        }
        if ((tokens[i]->type == REDIR_OUTPUT_APPEND || tokens[i]->type == REDIR_HEREDOC || tokens[i]->type == REDIR_IN) && tokens[i + 1]->type == PIPE)
        {
            return (i);
        }
        if ((tokens[i]->type == REDIR_OUTPUT_APPEND || tokens[i]->type == REDIR_HEREDOC || tokens[i]->type == REDIR_OUT || tokens[i]->type == REDIR_IN) && tokens[i + 1]->type == REDIR_OUT)
        {
            return (i);
        }
        if ((tokens[i]->type == REDIR_OUTPUT_APPEND || tokens[i]->type == REDIR_HEREDOC || tokens[i]->type == REDIR_OUT || tokens[i]->type == REDIR_IN) && tokens[i + 1]->type == REDIR_IN)
        {
            return (i);
        }
        if (tokens[i]->type == PIPE && tokens[i + 1]->type == PIPE)
        {
            return (i);
        }
        i++;
    }
    return (i);
}
