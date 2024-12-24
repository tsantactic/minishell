/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd_simple.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tambinin <tambinin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 11:25:57 by sandriam          #+#    #+#             */
/*   Updated: 2024/12/20 13:59:43 by tambinin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

const char	*get_env_value(const char *var, t_env **env_list);

void ft_execute_command(t_cmd *cmd, char *path, char **my_t_cmd, t_env **envp)
{
    char **env = env_list_to_array(*envp);
    
    execute_and_handle_error(cmd, path, my_t_cmd, env);
}

void execute_and_handle_error(t_cmd *cmd, char *path, char **my_t_cmd, char **env)
{
    if (execve(path, my_t_cmd, env) == -1)
    {
        struct stat path_stat;
        ft_putstr_fd("minishell: '", 2);
        ft_putstr_fd(my_t_cmd[0], 2);
        if (stat(my_t_cmd[0], &path_stat) == 0)
            handle_stat_error(my_t_cmd, cmd, env);
        else
            handle_errno_case(my_t_cmd, cmd);
        ft_free(env);
    }
    else
    {
        set_st(0);
        cleanup_resources(cmd, env);
    }
}

void handle_stat_error(char **my_t_cmd, t_cmd *cmd, char **env)
{
    int i = 0;
    int is_not_cmd = 0;
    int is_not_dir = 0;

    while (i < cmd->len_cmd)
    {
        if (ft_strchr(my_t_cmd[i], '.'))
        {
            is_not_cmd++;
            if (ft_strcmp(my_t_cmd[i], "..") == 0)
                is_not_cmd++;
        }
        if (ft_strchr(my_t_cmd[i], '/'))
            is_not_dir++;
        i++;
    }

    check_and_set_exit_status(my_t_cmd, is_not_cmd, is_not_dir, env);
}

void handle_errno_case(char **my_t_cmd, t_cmd *cmd)
{
    if (errno == 20)
    {
        ft_putstr_fd("': Not a directory\n", 2);
        set_st(126);
    }
    else if (errno == 2)
    {
        int i = 0;
        int is_not_cmd = 0;

        while (i < cmd->len_cmd)
        {
            if (ft_strchr(my_t_cmd[i], '/'))
            {
                is_not_cmd = 1;
                break;
            }
            i++;
        }

        if (is_not_cmd)
        {
            ft_putstr_fd("': No such file or directory\n", 2);
            set_st(127);
        }
        else
        {
            ft_putstr_fd("': command not found\n", 2);
            set_st(127);
        }
    }
}

void check_and_set_exit_status(char **my_t_cmd, int is_not_cmd, int is_not_dir, char **env)
{
    if (is_not_cmd == 1 && !is_not_dir && my_t_cmd[0][0] == '.')
    {
        ft_putstr_fd("': filename argument required\n.: usage: . filename [arguments]\n", 2);
        set_st(2);
    }
    else if (is_not_cmd == 2 && !is_not_dir)
    {
        ft_putstr_fd("':command not found\n", 2);
        set_st(127);
    }
    else if (is_not_cmd == 0 && is_not_dir == 0)
    {
        ft_putstr_fd("': command not found\n", 2);
        set_st(127);
    }
    else
    {                
        if (execve(my_t_cmd[0], my_t_cmd, env) == -1)
        {
            ft_putstr_fd("': Is a directoryy\n", 2);                           
            ft_free(env);
            set_st(126);
        } 
    }
}

void cleanup_resources(t_cmd *cmd, char **env)
{
    int i = 0;
    
    while (i < cmd->len_tokens)
    {
        free(cmd->tokens[i]->value);
        free(cmd->tokens[i]);
        i++;
    }

    free(cmd->tokens);
    ft_free(env);
}