#include "../../include/minishell.h"

void path_not_set_not_bin(char *command, t_env **env, char **tmp_cmd)
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
        if (is_not_cmd == 1 && !is_not_dir && tmp_cmd[0][0] == '.')
        {
            ft_putstr_fd("minishell: '", 2);
            ft_putstr_fd(tmp_cmd[0], 2);
            ft_putstr_fd("': filename argument required\n.: usage: . filename [arguments]\n", 2);
            set_st(2);
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
                    struct stat path_stat;
                    if (stat(tmp_cmd[0], &path_stat) == 0)
                    {
                        if (S_ISDIR(path_stat.st_mode))                            
                            ft_putstr_fd("': Is a directory\n", 2);                           
                        else if (S_ISREG(path_stat.st_mode))
                            perror("'");
                    }
                    ft_free(env_list);
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
            ft_putstr_fd("': No such file or directory\n", 2);
            set_st(127);
        }
    }
}