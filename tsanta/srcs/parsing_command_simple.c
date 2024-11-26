/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_command_simple.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sandriam <sandriam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 11:00:30 by sandriam          #+#    #+#             */
/*   Updated: 2024/11/26 11:56:06 by sandriam         ###   ########.fr       */
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
void loop_heredoc(char *delimiter, int *pipefd)
{
    char *input = NULL;
    while (1)
    {
        input = readline("heredoc>");
        if (!input)
            break;
        if (ft_strcmp(input, delimiter) == 0)
        {
            free(input);
            close(pipefd[1]);
            break;
        }
        ft_putendl_fd(input, pipefd[1]);
        free(input);
    }
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
                loop_heredoc(delimiter, pipe_heredoc[heredoc_index]);
                if (heredoc_index + 1 == count_heredoc)
                {
                    dup2(pipe_heredoc[heredoc_index][0], STDIN_FILENO);
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
                cmd->state = 1;
				set_state(cmd->state);
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
                cmd->state = 1;
				set_state(cmd->state);
                perror(cmd->tokens[k + 1]->value);                
                return (2);
            }
            dup2(fd, 0);
            close(fd);
            k++;
        }
        if (cmd->tokens[k]->type == REDIR_OUTPUT_APPEND)
        {
            int fd = open(cmd->tokens[k + 1]->value, O_WRONLY | O_APPEND | O_CREAT, 0777);
            if (fd < 0)
            {
                cmd->state = 1;
				set_state(cmd->state);
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

void parsing_argument_simple(t_cmd *cmd, char **env)
{
    /*parse redirection heredoc*/
    int **pipe_heredoc;
    int count_heredoc = 0;
    if (contains_heredoc(cmd))
    {
        count_heredoc = count_heredoc_arg(cmd, count_heredoc);
        pipe_heredoc = malloc(sizeof(int *) * count_heredoc);
        int i = 0;
        while (i < count_heredoc)
            pipe_heredoc[i++] = malloc(sizeof(int) * 2);
        execute_heredoc(cmd, pipe_heredoc, count_heredoc);
    }
    /*parse redirection in , out*/
    if (contains_redirection(cmd))
    {
        if (redirection_exec(cmd) == 2)
        {
            if (contains_heredoc(cmd))
            {
                int i = 0;
                while (i < count_heredoc)
                    free(pipe_heredoc[i++]);
                free(pipe_heredoc);
            }
            int	j;
            j = 0;
            while (j < cmd->len_arg)
            {
                free(cmd->token_arg[j]);
                j++;
            }
            free(cmd->token_arg);
            cmd->token_arg = NULL;
            j = 0;
            while (cmd->args[j])
            {
                free(cmd->args[j]);
                j++;
            }
            free(cmd->args);
            free_tokens(cmd);
            exit (1);
        }
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
                    ft_putstr_fd("': filename argument required\n.: usage: . filename [arguments]\n", 2);
                    cmd->state = 2;
                    set_state(cmd->state);
                }
                else if (is_not_cmd == 2 && !is_not_dir)
                {
                    ft_putstr_fd("':command not found\n", 2);
                    cmd->state = 127;
                    set_state(cmd->state);
                }
                else
                {
                    ft_putstr_fd("': Is a directory\n", 2);
                    cmd->state = 126;
                    set_state(cmd->state);
                }
            }
            else
            {
                if(errno == 20)
                {
                    ft_putstr_fd("': Not a directory\n", 2);
                    cmd->state = 126;
                    set_state(cmd->state);
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
                        cmd->state = 127;
                        set_state(cmd->state);
                    }
                    else
                    {
                        ft_putstr_fd("': command not found\n", 2);
                        cmd->state = 127;
                        set_state(cmd->state);
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
    set_state(-1);
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
