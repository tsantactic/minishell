/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_command_simple.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tambinin <tambinin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 11:00:30 by sandriam          #+#    #+#             */
/*   Updated: 2024/12/20 13:59:54 by tambinin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void parsing_argument_simple(t_cmd *cmd, t_env **env)
{
    /*parse redirection heredoc*/
    cmd->env_lst = env;
    parse_exec_heredoc(cmd);
    if (set_sig_heredoc(-1) == 1)
    {
        set_sig_heredoc(0);
        free_tokens(cmd);
        free_pipe_heredoc(cmd->count_heredoc, cmd->pipe_heredoc);
        return;
    }
    /*parse redirection in , out*/
    parse_exec_redirection(cmd);
    /*parse command*/
    char **tmp_cmd = NULL;
    int len_command = 0;
    char *command = NULL;
    count_command_arg(cmd, &len_command);
    tmp_cmd = malloc(sizeof(char *) * (len_command + 1));
    if (!tmp_cmd)
    {
        return ;
    }
    cmd->len_cmd = len_command;
    command = copy_command_arg(cmd, tmp_cmd, command);
    if (command)
    {
        char *extracted_command = extract_command_bin(command);
        char *path;
        if (check_env(env, "PATH=") == 1)
        {
            path = ft_find_path(extracted_command, env);
            path_is_here(path, env, tmp_cmd, command , cmd);
        }
        else
        {
            path_not_set(command, env, tmp_cmd);            
        }
        
        free(command);
        free(extracted_command);
        if (contains_heredoc(cmd))
            free_pipe_heredoc(cmd->count_heredoc, cmd->pipe_heredoc);
    }
    else
    {
        if (contains_heredoc(cmd))
            free_pipe_heredoc(cmd->count_heredoc, cmd->pipe_heredoc);
        free_tokens(cmd);
        if (tmp_cmd)
            ft_free(tmp_cmd);
        return;
    }
    free_tokens(cmd);
    if (tmp_cmd)
        ft_free(tmp_cmd);
    return ;
}
