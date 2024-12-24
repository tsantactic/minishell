/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_with_pipe.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tambinin <tambinin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 10:16:09 by sandriam          #+#    #+#             */
/*   Updated: 2024/12/20 15:23:05 by tambinin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void count_cmd_per_arg(t_cmd *cmd)
{
    int i = 0;
    int len = 0;
    int len_cmd = 0;
    int j = 0;
    while (i <= cmd->nb_pipe)
    {
        j = 0;
        len = 0;
        len_cmd  = 0;
        cmd->commands_arg[i][0]->command_value = NULL;
        while (cmd->commands_arg[i][j] && cmd->commands_arg[i][j]->value)
        {
            if (cmd->commands_arg[i][j]->type == ARG || cmd->commands_arg[i][j]->type == CMD)
            {
                if (cmd->commands_arg[i][j]->type_env == IS_ENV && ft_strchr(cmd->commands_arg[i][j]->value, ' ') != 0)
                {
                    char **split_env = ft_split(cmd->commands_arg[i][j]->value, ' ');
                    int count = 0;
                    while (split_env && split_env[count])
                    {
                        free(split_env[count]);
                        count++;
                    }
                    free(split_env);
                    len += count;
                }
                else
                {
                    if (cmd->commands_arg[i][j]->type == CMD)
                        cmd->commands_arg[i][0]->command_value = ft_strdup(cmd->commands_arg[i][j]->value);
                    len++;    
                }
            }
            len_cmd++;
            j++;
        }
        cmd->commands_arg[i][0]->len_cmd_arg = len;
        cmd->commands_arg[i][0]->total_len = len_cmd;
        cmd->commands_arg[i][0]->tmp_cmd_arg = malloc(sizeof(char *) * (cmd->commands_arg[i][0]->len_cmd_arg + 1));
        j = 0;
        int k = 0;
        while (cmd->commands_arg[i][j] && cmd->commands_arg[i][j]->value)
        {
            if (cmd->commands_arg[i][j]->type == ARG || cmd->commands_arg[i][j]->type == CMD)
            {
                if (cmd->commands_arg[i][j]->type_env == IS_ENV && ft_strchr(cmd->commands_arg[i][j]->value, ' ') != 0)
                {
                    char **split_env = ft_split(cmd->commands_arg[i][j]->value, ' ');
                    int index = 0;
                    while (split_env && split_env[index])
                    {
                        cmd->commands_arg[i][0]->tmp_cmd_arg[k++] = ft_strdup(split_env[index]);
                        if (index == 0)
                            cmd->commands_arg[i][0]->command_value = ft_strdup(split_env[0]);
                        free(split_env[index]);
                        index++;
                    }
                    free(split_env);
                }
                else
                    cmd->commands_arg[i][0]->tmp_cmd_arg[k++] = ft_strdup(cmd->commands_arg[i][j]->value);
            }
            j++;
        }
        cmd->commands_arg[i][0]->tmp_cmd_arg[k] = NULL;
        i++;
    }
}

void free_commands(t_cmd *cmd)
{
    int i = 0;
    int j = 0;

    while (i <= cmd->nb_pipe)
    {
        j = 0;
        if (cmd->commands_arg[i][0]->command_value)
            free(cmd->commands_arg[i][0]->command_value);

        ft_free(cmd->commands_arg[i][0]->tmp_cmd_arg);

        j = 0;
        while (cmd->commands_arg[i][j]) 
        {
            free(cmd->commands_arg[i][j]);
            j++;
        }
        free(cmd->commands_arg[i]);
        i++;
    }
}
void init_value_command(t_cmd *cmd, t_env **env)
{
    int i = 0;
    while (i <= cmd->nb_pipe)
	{
        cmd->commands_arg[i][0]->env_lst = env;
        cmd->commands_arg[i][0]->count_heredoc = 0;
        cmd->commands_arg[i][0]->fd_heredoc = -1;
        i++;
    }
}

void execute_heredoc_pipe(t_cmd *cmd)
{
    int i = 0;
    while (i <= cmd->nb_pipe)
	{
        parse_exec_heredoc_pipe(cmd->commands_arg[i], cmd->commands_arg[i][0]->total_len);
        if (set_st(-1) == 130)
            break;
        i++;
    }
}
void	handle_pipe(t_cmd *cmd, t_env **env)
{
	int	i;
	char	**envp;
	char	**args;

    cmd->fd_pipe = NULL;
    check_stdin(cmd);
    check_stdout(cmd);
	init_value_command(cmd, env);
    execute_heredoc_pipe(cmd);
    if (set_sig_heredoc(-1) == 1)
    {
        i = 0;
        while (i <= cmd->nb_pipe)
        {
            if (contains_heredoc_pipe(cmd->commands_arg[i], cmd->commands_arg[i][0]->total_len) && cmd->commands_arg[i][0]->fd_heredoc != -1)
            {
                if (cmd->commands_arg[i][0]->fd_heredoc != -1)
                    close(cmd->commands_arg[i][0]->fd_heredoc);
            }
            i++;
        }
        write(STDIN_FILENO, "\n", 1);
        set_sig_heredoc(0);        
    }
    else
    {
        cmd->fd_pipe = malloc(sizeof(int *) * cmd->nb_pipe);
        cmd->pid = NULL;
        if (!cmd->fd_pipe)
        {
            perror("malloc");
            exit(EXIT_FAILURE);
        }
        cmd->pid = malloc(sizeof(pid_t) *  (cmd->nb_pipe + 1));
        i = 0;
        while (i < cmd->nb_pipe)
        {
            cmd->fd_pipe[i] = malloc(sizeof(int) * 2);
            if (pipe(cmd->fd_pipe[i]) == -1)
            {
                perror("pipe");
                exit(EXIT_FAILURE);
            }
            i++;
        }
        i = 0;
        while (i <= cmd->nb_pipe)
        {
            cmd->pid[i] = fork();
            if (cmd->pid[i] == -1)
            {
                perror("fork");
                exit(EXIT_FAILURE);
            }
            if (cmd->pid[i] == 0) // child process
            {
                // rediriger l'entree standart si ce n'est pas la 1ere cmd
                if (i > 0)
                {
                    if (cmd->commands_arg[i][0]->stdin_set == 1)
                    {
                        if (dup2(cmd->fd_pipe[i - 1][0], STDIN_FILENO) == -1)
                        {
                            perror("dup2");
                            exit(EXIT_FAILURE);
                        }
                    }
                    else
                    {
                        close(cmd->fd_pipe[i - 1][0]);
                    }
                }
                if (i < cmd->nb_pipe)
                {
                    if (dup2(cmd->fd_pipe[i][1], STDOUT_FILENO) == -1)
                    {
                        perror("dup2");
                        exit(EXIT_FAILURE);
                    }
                }
                if (contains_heredoc_pipe(cmd->commands_arg[i], cmd->commands_arg[i][0]->total_len) && cmd->commands_arg[i][0]->fd_heredoc != -1)
                {
                    if (cmd->commands_arg[i][0]->fd_heredoc)
                    {
                        dup2(cmd->commands_arg[i][0]->fd_heredoc, STDIN_FILENO);
                        close(cmd->commands_arg[i][0]->fd_heredoc);
                    }
                }            
                if (parse_exec_redir_pipe(cmd->commands_arg[i], cmd->commands_arg[i][0]->total_len) == 1)
                {
                    int k = 0;
                    while (k <= cmd->nb_pipe)
                    {
                        if (contains_heredoc_pipe(cmd->commands_arg[k], cmd->commands_arg[k][0]->total_len) && cmd->commands_arg[k][0]->fd_heredoc != -1)
                        {
                            if (cmd->commands_arg[k][0]->fd_heredoc != -1)
                                close(cmd->commands_arg[k][0]->fd_heredoc);
                        }
                        k++;
                    }
                    int	j = 0;
                    while (j < cmd->nb_pipe)
                    {
                        close(cmd->fd_pipe[j][0]);
                        close(cmd->fd_pipe[j][1]);
                        j++;
                    }
                    for (i = 0; i < cmd->nb_pipe; i++)
                    {
                        free(cmd->fd_pipe[i]);
                    }
                    free(cmd->fd_pipe);	
                    free(cmd->pid);
                    ft_free_token_cmd(cmd);
                    free_tokens(cmd);
                    free_new_env(env);
                    free_commands(cmd);
                    free(cmd->commands_arg);
                    exit(1);
                }
                // if (i == num_pipes)
                //     printf("ici dernier commande on doit freeer le [%d]", cmd->first_heredoc);
                int	j = 0;
                while (j < cmd->nb_pipe)
                {
                    close(cmd->fd_pipe[j][0]);
                    close(cmd->fd_pipe[j][1]);
                    j++;
                }
                // printf("command_value : [%s]\n", cmd->commands_arg[i][0]->command_value);
                if (cmd->commands_arg[i][0]->command_value)
                {
                    args = cmd->commands_arg[i][0]->tmp_cmd_arg;
                    envp = env_list_to_array(*env);
                    // printf("1[%s]\n", cmd->commands_arg[i][0]->command_value);
                    char *extract_command = extract_command_bin(cmd->commands_arg[i][0]->command_value);
                    // printf("2[%s]\n", extract_command_bin(cmd->commands_arg[i][0]->command_value));
                    if (is_builtin(extract_command))
                    {
                        ft_free(envp);
                        int k = 0;
                        while (k <= cmd->nb_pipe)
                        {
                            if (contains_heredoc_pipe(cmd->commands_arg[k], cmd->commands_arg[k][0]->total_len) && cmd->commands_arg[k][0]->fd_heredoc != -1)
                            {
                                if (cmd->commands_arg[k][0]->fd_heredoc != -1)
                                    close(cmd->commands_arg[k][0]->fd_heredoc);
                            }
                            k++;
                        }
                        if (i != cmd->nb_pipe)
                        {
                            if (cmd->commands_arg[i + 1][0]->stdin_set == 1 || cmd->commands_arg[i][0]->stdout_set == 1)
                            {
                                if (ft_strcmp(extract_command, "echo") == 0)
                                {
                                    ft_echo(args);
                                }
                                if (ft_strcmp(extract_command, "pwd") == 0)
                                    ft_pwd(args);
                                if (ft_strcmp(extract_command, "env") == 0)
                                    ft_env(args, env);
                                if (ft_strcmp(extract_command, "export") == 0)
                                    ft_export(args, env);
                                if (ft_strcmp(extract_command, "unset") == 0)
                                    ft_unset(args, env);
                                if (ft_strcmp(extract_command, "cd") == 0)
                                    ft_cd(args, env);
                                if (ft_strcmp(extract_command, "exit") == 0)
                                    ft_exit_pipe(args, cmd, env, extract_command);
                            }
                        }
                        else
                        {
                            if (ft_strcmp(extract_command, "echo") == 0)
                                ft_echo(args);
                            if (ft_strcmp(extract_command, "pwd") == 0)
                                ft_pwd(args);
                            if (ft_strcmp(extract_command, "env") == 0)
                                ft_env(args, env);
                            if (ft_strcmp(extract_command, "export") == 0)
                                ft_export(args, env);
                            if (ft_strcmp(extract_command, "unset") == 0)
                                ft_unset(args, env);
                            if (ft_strcmp(extract_command, "cd") == 0)
                                ft_cd(args, env);
                            if (ft_strcmp(extract_command, "exit") == 0)
                                ft_exit_pipe(args, cmd, env, extract_command);
                        }                    
                        ft_free_token_cmd(cmd);
                        free_tokens(cmd);
                        free_commands(cmd);
                        free(cmd->commands_arg);
                        free(extract_command);
                        free_new_env(env);
                        for (int k = 0; k < cmd->nb_pipe; k++)
                        {
                            free(cmd->fd_pipe[k]);
                        }
                        free(cmd->fd_pipe);
                        free(cmd->pid);
                        exit(set_st(-1));
                    }
                    else
                    {
                        char	*path = ft_find_path(extract_command, env);
                        if (path)
                        {
                            signal(SIGQUIT, sig_quit_slash);
                            if (execve(path, args, envp) == -1)
                            {
                                int k = 0;
                                while (k <= cmd->nb_pipe)
                                {
                                    if (contains_heredoc_pipe(cmd->commands_arg[k], cmd->commands_arg[k][0]->total_len) && cmd->commands_arg[k][0]->fd_heredoc != -1)
                                    {
                                        if (cmd->commands_arg[k][0]->fd_heredoc != -1)
                                            close(cmd->commands_arg[k][0]->fd_heredoc);
                                    }
                                    k++;
                                }
                                int	j = 0;
                                while (j < cmd->nb_pipe)
                                {
                                    close(cmd->fd_pipe[j][0]);
                                    close(cmd->fd_pipe[j][1]);
                                    j++;
                                }
                                perror("execve");
                                ft_free_token_cmd(cmd);
                                free(path);
                                free_new_env(env);
                                ft_free(envp);
                                free_commands(cmd);
                                free_tokens(cmd);
                                free(extract_command);
                                free(cmd->commands_arg);
                                for (i = 0; i < cmd->nb_pipe; i++)
                                {
                                    free(cmd->fd_pipe[i]);
                                }
                                free(cmd->pid);
                                free(cmd->fd_pipe);
                                exit(set_st(127));
                            }
                        }
                        else
                        {
                            int k = 0;
                            while (k <= cmd->nb_pipe)
                            {
                                if (contains_heredoc_pipe(cmd->commands_arg[k], cmd->commands_arg[k][0]->total_len) && cmd->commands_arg[k][0]->fd_heredoc != -1)
                                {
                                    if (cmd->commands_arg[k][0]->fd_heredoc != -1)
                                        close(cmd->commands_arg[k][0]->fd_heredoc);
                                }
                                k++;
                            }
                            ft_putendl_fd(extract_command, 2);
                            ft_putendl_fd("minishell: command not found", STDERR_FILENO);
                            ft_free_token_cmd(cmd);
                            free_new_env(env);
                            ft_free(envp);                
                            free(extract_command);
                            free_commands(cmd);
                            free_tokens(cmd);
                            free(cmd->commands_arg);
                            for (i = 0; i < cmd->nb_pipe; i++)
                            {
                                free(cmd->fd_pipe[i]);
                            }
                            free(cmd->fd_pipe);
                            free(cmd->pid);
                            exit(set_st(127));
                        }   
                    }
                }
                else
                {
                    int k = 0;
                    while (k <= cmd->nb_pipe)
                    {
                        if (contains_heredoc_pipe(cmd->commands_arg[k], cmd->commands_arg[k][0]->total_len) && cmd->commands_arg[k][0]->fd_heredoc != -1)
                        {
                            if (cmd->commands_arg[k][0]->fd_heredoc != -1)
                                close(cmd->commands_arg[k][0]->fd_heredoc);
                        }
                        k++;
                    }

                    if (i > 0 && i < cmd->nb_pipe)
                    {
                        close(cmd->fd_pipe[i - 1][0]);
                    }
                    ft_free_token_cmd(cmd);
                    free_new_env(env);

                    free_commands(cmd);
                    free_tokens(cmd);
                    free(cmd->commands_arg);
                    for (i = 0; i < cmd->nb_pipe; i++)
                    {                   
                        free(cmd->fd_pipe[i]);
                    }
                    free(cmd->fd_pipe);	
                    free(cmd->pid);
                    exit(set_st(-1));        
                }
            }
            i++;
        }
        i = 0;
        int stat;
        int	j = 0;
        while (j < cmd->nb_pipe)
        {
            close(cmd->fd_pipe[j][0]);
            close(cmd->fd_pipe[j][1]);
            j++;
        }
        i = 0;
        while (i <= cmd->nb_pipe)
        {
            if (contains_heredoc_pipe(cmd->commands_arg[i], cmd->commands_arg[i][0]->total_len) && cmd->commands_arg[i][0]->fd_heredoc != -1)
            {
                if (cmd->commands_arg[i][0]->fd_heredoc != -1)
                    close(cmd->commands_arg[i][0]->fd_heredoc);
            }
            i++;
        }
        i = 0;
        while (i <= cmd->nb_pipe)
        {
            waitpid(cmd->pid[i], &stat, 0);
            i++;
        }
        if (WIFEXITED(stat))
        {
            set_st(WEXITSTATUS(stat));
        }
        else if (WIFSIGNALED(stat))
        {
            set_st(WTERMSIG(stat) + 128);
            if (set_st(WTERMSIG(stat) + 128) == 131)
                printf("quit (core dumped)\n");
        }
        i = -1;
        while (++i < cmd->nb_pipe)
        {
            free(cmd->fd_pipe[i]);
        }
        free(cmd->pid);
        free(cmd->fd_pipe);
    }
}

void execute_with_pipes(t_cmd *cmd, t_env *env[])
{
	
    int j = 0;
    int k = 0;
	int	index = 0;

    cmd->commands_arg = malloc(sizeof(t_token **) * (cmd->nb_pipe + 1));
    if (!cmd->commands_arg)
        return ;
    while (index < cmd->len_tokens)
    {
        if (cmd->tokens[index]->type == PIPE)
        {
            cmd->commands_arg[j][k] = NULL;
            j++;
            k = 0;
        }
        else
        {
            if (k == 0)
            {
                cmd->commands_arg[j] = malloc(sizeof(t_token *) * (cmd->len_tokens + 1));
                if (!cmd->commands_arg[j])
                    return;
            }
            cmd->commands_arg[j][k] = malloc(sizeof(t_token));
            if (!cmd->commands_arg[j][k])
                return;

            cmd->commands_arg[j][k]->value = cmd->tokens[index]->value;
            cmd->commands_arg[j][k]->type = cmd->tokens[index]->type;
            cmd->commands_arg[j][k]->type_env = cmd->tokens[index]->type_env;
            cmd->commands_arg[j][k]->type_quote_delim = cmd->tokens[index]->type_quote_delim;
            k++;
        }
        index++;
    }
    cmd->commands_arg[j][k] = NULL;
    count_cmd_per_arg(cmd);
	handle_pipe(cmd, env);
    free_commands(cmd);
    free(cmd->commands_arg);
}
