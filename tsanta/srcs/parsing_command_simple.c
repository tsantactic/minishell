/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_command_simple.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sandriam <sandriam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 11:00:30 by sandriam          #+#    #+#             */
/*   Updated: 2024/12/03 17:32:14 by sandriam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/****heredoc****/
int count_heredoc_arg(t_cmd *cmd, int count_heredoc)
{
    int i;

    i = 0;
    while (i < cmd->len_tokens)
    {
        if (cmd->tokens[i]->type == REDIR_HEREDOC)
            count_heredoc++;
        i++;
    }
    return (count_heredoc);
}

char *expand_heredoc(char *input, t_cmd *cmd)
{
    (void)cmd;
    char *result;
    int len = 0;
    int i = 0;
    int j = 0;
    
    while(i < (int)ft_strlen(input))
    {
        if (input[i] == '$' && ft_isdigit(input[i + 1]))
        {
            i += 2;
        }
        else if (input[i] == '$' && input[i + 1] != '\0' && input[i + 1] != '$')
        {
            int var_env = 0;
            while (ft_isalnum(input[i+1+var_env]) || input[i+1+var_env] == '_')
                var_env++;
            char *var_name = malloc(sizeof(char) * var_env + 1);
            ft_strncpy(var_name, (char *)&input[i + 1], var_env);
            var_name[var_env] = '\0';
            const char *value = get_env_value(var_name, cmd->env_lst);
            if (value)
            {
                len += ft_strlen(value);
            }
            free(var_name);
            i += var_env + 1;
        }
        else
        {
            i++;
            len++;
        }
    }
    result = malloc(sizeof(char) * (len + 1));
    i = 0;
    j = 0;
    while(input[i])
    {
        if (input[i] == '$' && ft_isdigit(input[i + 1]))
        {
            i += 2;
        }
        else if (input[i] == '$' && input[i+1] != '\0' && input[i + 1] != '$')
        {
            int var_env = 0;
            while (ft_isalnum(input[i+1+var_env]) || input[i+1+var_env] == '_')
                var_env++;
            char *var_name = malloc(sizeof(char) * var_env + 1);
            ft_strncpy(var_name, (char *)&input[i + 1], var_env);
            var_name[var_env] = '\0';
            const char *value = get_env_value(var_name, cmd->env_lst);
            if (value)
            {
                ft_strncpy(&result[j],(char*)value, ft_strlen(value));
                j += ft_strlen(value);
            }
            free(var_name);
            i += var_env + 1;
        }
        else
            result[j++] = input[i++];
    }
    result[len] = '\0';
    input = malloc(sizeof(char) * ft_strlen(result) + 1);
    i = 0;
    while (result[i])
    {
        input[i] = result[i];
        i++;
    }
    free(result);
    input[i] = '\0';
    return (input);
}

void loop_heredoc(char *delimiter, int *pipefd, t_cmd *cmd)
{
    char *input = NULL;
    set_sig_heredoc(0);
    cmd->stdin_heredoc = dup(STDIN_FILENO);
    while (1)
    {
        signal(SIGINT, sig_handler_heredoc);
        input = readline("heredoc>");
        if (set_sig_heredoc(-1) == 1)
        {
            if (input)
                free(input);
            close(pipefd[1]);
            break;
        }
        if (!input)
        {
            ft_putstr_fd("minishel : warning: here-document at this line delimited by end-of-file (wanted :'",2);
            ft_putstr_fd(delimiter, 2);
            ft_putstr_fd("')\n", 2);
            close(pipefd[1]);
            set_st(0);
            break;
        }
        if (ft_strcmp(input, delimiter) == 0)
        {
            free(input);
            close(pipefd[1]);
            set_st(0);
            break;
        }
        input = expand_heredoc(input, cmd);
        ft_putendl_fd(input, pipefd[1]);
        free(input);
    }
    dup2(cmd->stdin_heredoc, 0);
    close(cmd->stdin_heredoc);
}
void execute_heredoc(t_cmd *cmd, int **pipe_heredoc, int count_heredoc)
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
                loop_heredoc(delimiter, pipe_heredoc[heredoc_index], cmd);
                int k = i + 1;
                int is_last_stdin = 0;
                while (k < cmd->len_tokens)
                {
                    if (cmd->tokens[k]->type == REDIR_HEREDOC || cmd->tokens[k]->type == REDIR_IN)
                        is_last_stdin = 1;
                    k++;
                }
                if (heredoc_index + 1 == count_heredoc && is_last_stdin == 0)
                {
                    if (set_st(-1) != 130)
                    {
                        dup2(pipe_heredoc[heredoc_index][0], STDIN_FILENO);
                        close(pipe_heredoc[heredoc_index][1]);
                    }
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
int contains_redirection(t_cmd *cmd)
{
    int i = 0;
    while (i < cmd->len_arg)
    {
        if (cmd->tokens[i]->type == REDIR_IN)
            return (1);
        if (cmd->tokens[i]->type == REDIR_OUT)
            return (1);
        if (cmd->tokens[i]->type == REDIR_OUTPUT_APPEND)
            return (1);
        i++;
    }
    return (0);
}
int contains_heredoc(t_cmd *cmd)
{
    int i = 0;
    while (i < cmd->len_arg)
    {
        if (cmd->tokens[i]->type == REDIR_HEREDOC)
            return (1);
        i++;
    }
    return (0);
}
void parse_exec_heredoc(t_cmd *cmd)
{
    cmd->pipe_heredoc = NULL;
    cmd->count_heredoc = 0;

    if (contains_heredoc(cmd))
    {
        cmd->count_heredoc = count_heredoc_arg(cmd, cmd->count_heredoc);
        cmd->pipe_heredoc = malloc(sizeof(int *) * cmd->count_heredoc);
        int i = 0;
        while (i < cmd->count_heredoc)
            cmd->pipe_heredoc[i++] = malloc(sizeof(int) * 2);
        execute_heredoc(cmd, cmd->pipe_heredoc, cmd->count_heredoc);
    }
}
/****command****/
void count_command_arg(t_cmd *cmd, int *len_command)
{
    int i;
    i = 0;
    while (i < cmd->len_tokens)
    {
        if (cmd->tokens[i]->type == CMD || cmd->tokens[i]->type == ARG)
            (*len_command)++;
        i++;
    }
}
char  *copy_command_arg(t_cmd *cmd, char **my_t_cmd, char *command)
{
    int j = 0;
    int k = 0;
    while (k < cmd->len_tokens)
    {
        if (cmd->tokens[k]->type == ARG || cmd->tokens[k]->type == CMD)
        {
            if (cmd->tokens[k]->type == CMD)
                command = ft_strdup(cmd->tokens[k]->value);
            my_t_cmd[j++] = ft_strdup(cmd->tokens[k]->value);
        }
        k++;
    }
    my_t_cmd[j] = NULL;
    return(command);
}
int redirection_exec(t_cmd *cmd)
{
    int k = 0;
    while (k < cmd->len_tokens)
    {
        if (cmd->tokens[k]->type == REDIR_OUT)
        {
            int fd = open(cmd->tokens[k + 1]->value, O_WRONLY | O_CREAT | O_TRUNC, 0777);
            if (fd < 0)
            {
				set_st(1);
                perror(cmd->tokens[k + 1]->value);
                return (1);
            }
            dup2(fd, 1);
            close(fd);
            k++;
        }
        if (cmd->tokens[k]->type == REDIR_IN)
        {
            int fd = open(cmd->tokens[k + 1]->value, O_RDONLY);
            if (fd < 0)
            {
				set_st(1);
                perror(cmd->tokens[k + 1]->value);                
                return (2);
            }
            int i = k + 1;
            int is_last_stdin = 0;
            while (i < cmd->len_tokens)
            {
                if (cmd->tokens[i]->type == REDIR_HEREDOC || cmd->tokens[i]->type == REDIR_IN)
                    is_last_stdin = 1;
                i++;
            }
            if (is_last_stdin == 0)
                dup2(fd, 0);
            close(fd);
            k++;
        }
        if (cmd->tokens[k]->type == REDIR_OUTPUT_APPEND)
        {
            int fd = open(cmd->tokens[k + 1]->value, O_WRONLY | O_APPEND | O_CREAT, 0777);
            if (fd < 0)
            {
				set_st(1);
                perror(cmd->tokens[k + 1]->value);
                return (1);
            }
            dup2(fd, 1);
            close(fd);
            k++;
        }
        k++;
    }
    return (0);
}

void parse_exec_redirection(t_cmd *cmd)
{
    if (contains_redirection(cmd))
    {
        if (redirection_exec(cmd) == 2)
        {
            if (contains_heredoc(cmd))
                free_pipe_heredoc(cmd->count_heredoc, cmd->pipe_heredoc);
            ft_free_token_cmd(cmd);
            free_tokens(cmd);
            free_new_env(cmd->env_lst);
            exit (1);
        }
    }
}
int contains_bin(const char *command)
{
    return (ft_strncmp((char*)command, "/bin/", 5) == 0 || ft_strncmp((char*)command, "/usr/bin/", 9) == 0);
}

char *extract_command_bin(const char *command)
{
    const char *start;
    
    if (contains_bin(command))
    {
        if (command[4] == '/')
        {
            start = command + 5;
        }
        else
        {
            start = command + 9;
        }   
        return ft_strdup(start);
    }
    return ft_strdup(command);
}
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
    command = copy_command_arg(cmd, tmp_cmd, command);
    if (command)
    {
        char *extracted_command = extract_command_bin(command);
        char *path;
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
                    ft_putstr_fd("': filename argument required\n.: usage: . filename [arguments]\n", 2);
                    set_st(2);
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