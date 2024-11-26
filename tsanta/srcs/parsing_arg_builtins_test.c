/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_arg_builtins_test.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sandriam <sandriam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 09:26:57 by sandriam          #+#    #+#             */
/*   Updated: 2024/11/25 11:05:20 by sandriam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
void execute_heredoc_builtins(t_cmd *cmd, int **pipe_heredoc, int count_heredoc, int *temp_pipe_heredoc)
{
    int heredoc_index = 0;
    int i;
    char *delimiter;

    delimiter = NULL;
    i = 0;
    while (i < cmd->len_tokens)
    {   
        if (cmd->tokens[i]->type == REDIR_HEREDOC)
        {
            if (cmd->tokens[i + 1]->type == DELIMITER)
            {
                delimiter = cmd->tokens[i + 1]->value;
                pipe(pipe_heredoc[heredoc_index]);
                loop_heredoc(delimiter, pipe_heredoc[heredoc_index]);
                if (heredoc_index + 1 == count_heredoc)
                {
                    *temp_pipe_heredoc = pipe_heredoc[heredoc_index][0];
                    // dup2(pipe_heredoc[heredoc_index][0], STDIN_FILENO);
                    close(pipe_heredoc[heredoc_index][1]);
                    close(pipe_heredoc[heredoc_index][0]);
                }
                else
                    close(pipe_heredoc[heredoc_index][0]);
                heredoc_index++;
            }
            i += 1;
        }
        i++;
    }
}
void parsing_arg_with_builtins(t_cmd *cmd, char **env)
{
    /*parse redirection heredoc*/
    int **pipe_heredoc;
    int count_heredoc = 0;
    int temp_pipe_heredoc = 0;
    if (contains_heredoc(cmd))
    {
        count_heredoc = count_heredoc_arg(cmd, count_heredoc);
        pipe_heredoc = malloc(sizeof(int *) * count_heredoc);
        int i = 0;
        while (i < count_heredoc)
            pipe_heredoc[i++] = malloc(sizeof(int) * 2);
        execute_heredoc_builtins(cmd, pipe_heredoc, count_heredoc, &temp_pipe_heredoc);
    }
    /*parse redirection in , out*/
    if (contains_redirection(cmd))
    {
        redirection_exec(cmd);
    }

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
    command = copy_command_arg(cmd, tmp_cmd, command);
    if (command)
    {
        char *extracted_command = extract_command_bin(command);
        if (is_builtin(extracted_command))
        {
            if (contains_heredoc(cmd))
            {
                dup2(temp_pipe_heredoc, 0);
                close(temp_pipe_heredoc);
            }
            printf("is buitins\n");
            free(command);
            free(extracted_command);
            if (tmp_cmd)
                ft_free(tmp_cmd);
            if (contains_heredoc(cmd))
            {
                int i = 0;
                while (i < count_heredoc)
                {
                    free(pipe_heredoc[i]);
                    i++;
                }
                free(pipe_heredoc);
            }
            return ;
        }
        else
        {
            dup2(temp_pipe_heredoc, 0);
            close(temp_pipe_heredoc);
            char *path;
            path = ft_find_path(extracted_command, env);
            if (path)
            {
                ft_execute_command(cmd, path, tmp_cmd, env);
                free(path);
            }
            else
            {
                struct stat path_stat;
                ft_putstr_fd("minishell: '", 2);
                ft_putstr_fd(tmp_cmd[0], 2);
                if (stat(tmp_cmd[0], &path_stat) == 0)
                {
                    int i = 0;
                    int is_not_cmd= 0;
                    int is_not_dir = 0;
                    while (tmp_cmd[i])
                    {
                        if (ft_strchr(tmp_cmd[i], '.'))
                        {
                            is_not_cmd++;
                            if (ft_strcmp(tmp_cmd[i], "..") == 0)
                            {
                                is_not_cmd++;
                            }
                        }
                        if (ft_strchr(tmp_cmd[i], '/'))
                        {
                            is_not_dir++;
                        }
                        i++;
                    }
                    if (is_not_cmd == 1 && !is_not_dir)
                    {
                        ft_putstr_fd(": filename argument required\n.: usage: . filename [arguments]\n", 2);	
                    }
                    else if (is_not_cmd == 2 && !is_not_dir)
                    {
                        ft_putstr_fd(":command not found\n", 2);
                    }
                    else
                    {
                        ft_putstr_fd(": Is a directory\n", 2);
                    }
                }
                else
                {
                    if(errno == 20)
                    {
                        ft_putstr_fd(": Not a directory\n", 2);
                    }
                    else if (errno == 2)
                    {
                        int i = 0;
                        int is_not_cmd = 0;
                        while (tmp_cmd[i])
                        {
                            if (ft_strchr(tmp_cmd[i], '/'))
                            {
                                is_not_cmd = 1;
                                break;
                            }
                            i++;
                        }
                        if (is_not_cmd)
                        {
                            ft_putstr_fd(": No such file or directory\n", 2);
                        }
                        else
                        {
                            ft_putstr_fd(": command not found\n", 2);
                        }
                    }
                }
            }
        }
        free(command);
        free(extracted_command);
        if (contains_heredoc(cmd))
        {
            int i = 0;
            while (i < count_heredoc)
            {
                free(pipe_heredoc[i]);
                i++;
            }
            free(pipe_heredoc);
        }
    }
    else
    {
        if (contains_heredoc(cmd))
        {
            int i = 0;
            while (i < count_heredoc)
            {
                free(pipe_heredoc[i]);
                i++;
            }
            free(pipe_heredoc);
        }

        free_tokens(cmd);
        if (tmp_cmd)
            ft_free(tmp_cmd);
        return;
    }
    int i = 0;
    while (i < cmd->len_tokens)
    {
        free(cmd->tokens[i]->value);
        free(cmd->tokens[i]);
        i++;
    }
    free(cmd->tokens);
    if (tmp_cmd)
        ft_free(tmp_cmd);
    return ;
}
