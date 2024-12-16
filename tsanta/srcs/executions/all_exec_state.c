/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   all_exec_state.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sandriam <sandriam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/12 10:21:38 by sandriam          #+#    #+#             */
/*   Updated: 2024/12/12 10:40:24 by sandriam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void path_is_here(char *path, t_env **env, char **tmp_cmd, char *command, t_cmd *cmd)
{
    if (path && !ft_strchr(command, '.') && !ft_strchr(command, '/'))
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
            else if ((is_not_cmd == 0 && is_not_dir == 0) || (is_not_cmd == 2 && !is_not_dir))
            {
                ft_putstr_fd("minishell: '", 2);
                ft_putstr_fd(tmp_cmd[0], 2);
                ft_putstr_fd("': command not found\n", 2);
                set_st(127);
            }
            else
            {
                char **env_list = env_list_to_array(*env);
                
                if (execve(command, tmp_cmd, env_list) == -1)
                {
                    ft_putstr_fd("minishell: '", 2);
                    ft_putstr_fd(tmp_cmd[0], 2);
                    if (S_ISDIR(path_stat.st_mode))                            
                        ft_putstr_fd("': Is a directory\n", 2);                           
                    else if (S_ISREG(path_stat.st_mode))
                        perror("'");                    
                    ft_free(env_list);
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

void path_not_set(char *command, t_env **env, char **tmp_cmd)
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
                    ft_free(cmd);
                    ft_free(env_term);
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
            char **env_list = env_list_to_array(*env);
            if (execve(command, tmp_cmd, env_list) == -1)
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
                ft_free(env_list);
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
                ft_putstr_fd("': Is a directory\n", 2);
                set_st(126);
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
                        char *cmd[] = {"/bin/clear", NULL};
                        char **env_list = env_list_to_array(*env);
                        if (execve("/bin/clear", cmd, env_list) == -1)
                        {                                    
                            ft_free(cmd);
                            ft_free(env_list);
                            ft_putstr_fd("': Is a directory\n", 2);
                            set_st(126);
                        }
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
                    char **env_list = env_list_to_array(*env);

                    if (execve(join_path, tmp_cmd, env_list) == -1)
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
                    ft_putstr_fd("': No such file or directory\n", 2);
                    set_st(127);                            
                }
            }
        }
    }
}