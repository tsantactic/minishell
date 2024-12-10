/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sandriam <sandriam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 10:29:04 by sandriam          #+#    #+#             */
/*   Updated: 2024/12/09 18:56:35 by sandriam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/****heredoc****/
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

int count_len_expand_value(t_cmd *cmd, char *input)
{
    int i = 0;
    int len = 0;
    int var_env;
    char *var_name;
    const char *value;

    while(i < (int)ft_strlen(input))
    {
        if (input[i] == '$' && ft_isdigit(input[i + 1]))
        {
            i += 2;
        }
        else if (input[i] == '$' && input[i + 1] != '\0' && input[i + 1] != '$')
        {
            var_env = 0;
            while (ft_isalnum(input[i+1+var_env]) || input[i+1+var_env] == '_')
                var_env++;
            var_name = malloc(sizeof(char) * var_env + 1);
            ft_strncpy(var_name, (char *)&input[i + 1], var_env);
            var_name[var_env] = '\0';
            value = get_env_value(var_name, cmd->env_lst);
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
    return (len);
}

char *expand_heredoc(char *input, t_cmd *cmd)
{
    (void)cmd;
    char *result;
    int len;
    int i;
    int j;

    len = count_len_expand_value(cmd, input);
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

void loop_heredoc(char *delimiter, int *pipefd, t_cmd *cmd, int type_quote_delim)
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
        if (type_quote_delim == DELIMITER_NO_QUOTE)
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
        cmd->type_del = -1;
        if (cmd->tokens[i]->type == REDIR_HEREDOC)
        {
            if (cmd->tokens[i + 1]->type == DELIMITER)
            {
                delimiter = cmd->tokens[i + 1]->value;
                pipe(pipe_heredoc[heredoc_index]);
                cmd->type_del = cmd->tokens[i + 1]->type_quote_delim;
                loop_heredoc(delimiter, pipe_heredoc[heredoc_index], cmd, cmd->type_del);
                if (set_sig_heredoc(-1) == 1)
                {
                    close(pipe_heredoc[heredoc_index][0]);
                    break;
                }
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
