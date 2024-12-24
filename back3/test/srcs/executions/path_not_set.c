#include "../../include/minishell.h"

void path_not_set_bin(char *command, t_env **env, char **tmp_cmd)
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