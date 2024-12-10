/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_command_simple.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sandriam <sandriam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 11:00:30 by sandriam          #+#    #+#             */
/*   Updated: 2024/12/10 17:52:35 by sandriam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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
            if (path)
            {
                signal(SIGQUIT, sig_quit_slash);
                ft_execute_command(cmd, path, tmp_cmd, env);
                free(path);
            }
            else
            {
                struct stat path_stat;
                
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
                        if (tmp_cmd[0][0] == '.')
                        {
                            ft_putstr_fd("minishell: '", 2);
                            ft_putstr_fd(tmp_cmd[0], 2);
                            ft_putstr_fd("': filename argument required\n.: usage: . filename [arguments]\n", 2);
                            set_st(2);
                        }
                        else
                        {
                            ft_putstr_fd("minishell: '", 2);
                            ft_putstr_fd(tmp_cmd[0], 2);
                            ft_putstr_fd("': command not found\n", 2);
                            set_st(127);
                        }
                    }
                    else if (is_not_cmd == 2 && !is_not_dir)
                    {
                        ft_putstr_fd("minishell: '", 2);
                        ft_putstr_fd(tmp_cmd[0], 2);
                        ft_putstr_fd("': command not found\n", 2);
                        set_st(127);
                    }
                    else if (is_not_cmd == 0 && is_not_dir == 0)
                    {
                        ft_putstr_fd("minishell: '", 2);
                        ft_putstr_fd(tmp_cmd[0], 2);
                        ft_putstr_fd("': command not found\n", 2);
                        set_st(127);
                    }
                    else
                    {
                        if (execve(command, tmp_cmd, NULL) == -1)
                        {
                            ft_putstr_fd("minishell: '", 2);
                            ft_putstr_fd(tmp_cmd[0], 2);
                            if (errno == 13)
                                perror("'");  /*permission denied*/                         
                            else
                                ft_putstr_fd("': Is a directory\n", 2);
                            set_st(126);
                        }
                    }
                }
                else
                {
                    ft_putstr_fd("minishell: '", 2);
                    if(errno == 20)
                    {
                        ft_putstr_fd(tmp_cmd[0], 2);
                        ft_putstr_fd("': Not a directory\n", 2);
                        set_st(126);
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
                            ft_putstr_fd(tmp_cmd[0], 2);
                            ft_putstr_fd("': No such file or directory\n", 2);
                            set_st(127);
                        }
                        else
                        {   
                            cmd->tmp_value = copy_command(cmd, command);                    
                            ft_putstr_fd(cmd->tmp_value, 2);
                            ft_putstr_fd("': command not found\n", 2);
                            free(cmd->tmp_value);
                            set_st(127);
                        }
                    }
                }
            }
        }
        else
        {
            if (contains_bin(command))
            {
                if (ft_strcmp(command, "/usr/bin/clear") == 0 || ft_strcmp(command, "/bin/clear") == 0)
                {
                    if (check_env(env, "TERM=") == 1)
                    {
                        char *env_term[] = {"TERM=xterm", NULL};                        
                        char *cmd[] = {"/bin/clear", NULL};
                        if (execve("/bin/clear", cmd, env_term) == -1) 
                        {
                            perror("execve failed");
                            exit(1);
                        }
                        ft_free(env_term);
                    }
                    else
                    {
                        if (execve("/bin/clear", tmp_cmd, NULL) == -1)
                        {
                            perror("execve failed");
                            exit(1);
                        }
                    }
                }
                else
                {
                    if (execve(command, tmp_cmd, NULL) == -1)
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
                                if (tmp_cmd[0][0] == '.')
                                {
                                    ft_putstr_fd("': filename argument required\n.: usage: . filename [arguments]\n", 2);
                                    set_st(2);
                                }
                                else
                                {
                                    ft_putstr_fd("':command not found\n", 2);
                                    set_st(127);
                                }
                            }
                            else if (is_not_cmd == 2 && !is_not_dir)
                            {
                                ft_putstr_fd("':command not found\n", 2);
                                set_st(127);
                            }
                            else
                            {
                                ft_putstr_fd("': Is a directory\n", 2);
                                set_st(126);
                            }
                        }
                        else
                        {
                            if(errno == 20)
                            {
                                ft_putstr_fd("': Not a directory\n", 2);
                                set_st(126);
                            }
                            else if (errno == 2)
                            {
                                int i = 0;
                                int is_not_cmd = 0;
                                while (tmp_cmd[i])
                                {
                                    if (ft_strchr(tmp_cmd   [i], '/'))
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
                    }
                    else
                    {
                        set_st(0);
                    }
                }
            }
            else
            {
                struct stat path_stat;
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
                        ft_putstr_fd("minishell: '", 2);
                        ft_putstr_fd(command, 2);
                        if (tmp_cmd[0][0] == '.')
                        {
                            ft_putstr_fd("': filename argument required\n.: usage: . filename [arguments]\n", 2);
                            set_st(2);
                        }
                        else
                        {
                            ft_putstr_fd("':command not found\n", 2);
                            set_st(127);
                        }
                    }
                    else if (is_not_cmd == 2 && !is_not_dir)
                    {
                        ft_putstr_fd("minishell: '", 2);
                        ft_putstr_fd(command, 2);
                        ft_putstr_fd("':command not found\n", 2);
                        set_st(127);
                    }
                    else
                    {
                        char cwd[1024];
                        char *join_path = ft_strcat(getcwd(cwd, sizeof(cwd)), "/");
                        join_path = ft_strcat(join_path, command);
                        if (ft_strcmp(join_path, "/usr/bin/clear") == 0 || ft_strcmp(join_path, "/bin/clear") == 0)
                        {
                            if (check_env(env, "TERM=") == 1)
                            {
                                char *env_term[] = {"TERM=xterm", NULL};                        
                                char *cmd[] = {"/bin/clear", NULL};
                                if (execve("/bin/clear", cmd, env_term) == -1)
                                {
                                    ft_putstr_fd("': Is a directory\n", 2);
                                    set_st(126);
                                }
                                ft_free(env_term);
                            }
                            else
                            {
                                if (execve("/bin/clear", tmp_cmd, NULL) == -1)
                                {
                                    perror("execve failed");
                                    set_st(1);
                                }
                            }
                        }
                        else
                        {
                            if (execve(join_path, tmp_cmd, NULL) == -1)
                            {
                                ft_putstr_fd("minishell: '", 2);
                                ft_putstr_fd(command, 2);
                                ft_putstr_fd("': Is a directory\n", 2);
                                set_st(126);
                            }
                        }   
                    }
                }
                else
                {
                    ft_putstr_fd("minishell: '", 2);
                    ft_putstr_fd(tmp_cmd[0], 2);
                    if(errno == 20)
                    {
                        ft_putstr_fd("': Not a directory\n", 2);
                        set_st(126);
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
            }
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