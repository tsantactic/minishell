/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_with_pipe.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tambinin <tambinin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 11:26:30 by tambinin          #+#    #+#             */
/*   Updated: 2024/12/20 14:40:22 by tambinin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <unistd.h>

void    parse_exec_heredoc_pipe(t_token **commands_arg, int len)
{
    if (contains_heredoc_pipe(commands_arg, len))
    {
        int stdin_heredoc = dup(STDIN_FILENO);
        commands_arg[0]->count_heredoc = count_heredoc_pipe(commands_arg, commands_arg[0]->count_heredoc, len);
        execute_pipe_heredoc(commands_arg, commands_arg[0]->pipe_heredoc, len, commands_arg[0]->count_heredoc);
        dup2(stdin_heredoc, 0);
        close(stdin_heredoc);
    }
}
int contains_heredoc_pipe(t_token **token, int len)
{
    int i = 0;
    
    while (i < len)
    {
        if (token[i]->type == REDIR_HEREDOC)
            return  (1);
        i++;
    }
    return (0);
}
int count_heredoc_pipe(t_token **token, int count_heredoc, int len)
{
    int i = 0;
    while (i < len)
    {
        if (token[i]->type == REDIR_HEREDOC)
            count_heredoc++;
        i++;
    }
    return (count_heredoc);
}
int count_len_expand_value_pipe(t_env **env, char *input)
{
    int i = 0;
    int len = 0;
    int var_env;
    char *var_name;
    const char *value;

    while(i < (int)ft_strlen(input))
    {
        if (input[i] == '$' && input[i + 1] == '$')
        {
            i += 2;
            len += 2;
        }
        else if (input[i] == '$' && ft_isdigit(input[i + 1]))
        {
            i += 2;
        }
        else if (input[i] == '$' && input[i + 1] != '\0' && input[i + 1] != '$' && input[i + 1] != ' ' && ft_isalnum(input[i + 1]))
        {
            var_env = 0;
            while (ft_isalnum(input[i+1+var_env]) || input[i+1+var_env] == '_')
                var_env++;
            var_name = malloc(sizeof(char) * var_env + 1);
            ft_strncpy(var_name, (char *)&input[i + 1], var_env);
            var_name[var_env] = '\0';
            value = get_env_value(var_name, env);
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

char *expand_heredoc_pipe(char *input, t_env **env)
{
    char *result;
    int len;
    int i;
    int j;
    
    len = count_len_expand_value_pipe(env, input);
    result = malloc(sizeof(char) * (len + 1));
    i = 0;
    j = 0;
    while(input[i])
    {
        if (input[i] == '$' && input[i + 1] == '$')
        {
            result[j++] = '$';
            result[j++] = '$';
            i += 2;
        }
        else if (input[i] == '$' && ft_isdigit(input[i + 1]))
        {
            i += 2;
        }
        else if (input[i] == '$' && input[i+1] != '\0' && input[i + 1] != '$' && input[i + 1] != ' ' && ft_isalnum(input[i + 1]))
        {
            int var_env = 0;
            while (ft_isalnum(input[i+1+var_env]) || input[i+1+var_env] == '_')
                var_env++;
            char *var_name = malloc(sizeof(char) * var_env + 1);
            ft_strncpy(var_name, (char *)&input[i + 1], var_env);
            var_name[var_env] = '\0';
            const char *value = get_env_value(var_name, env);
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

void looop(char *delimiter, int *fd, int is_last_stdin, t_token **token)
{
    char *input = NULL;
    set_sig_heredoc(0);
    while (1)
    {
        signal(SIGINT, sig_handler_heredoc);
		input = readline("heredoc>");
		if (set_sig_heredoc(-1) == 1) // ctrl + C
		{
			if (input)
				free(input);
            if (is_last_stdin == 0)
                close(fd[1]);
			break;
		}
		if (!input) // ctrl + D
		{
			ft_putstr_fd("minishel : warning: here-document at this line delimited by end-of-file (wanted :'",2);
            ft_putstr_fd(delimiter, 2);
            ft_putstr_fd("')\n", 2);
            if (is_last_stdin == 0)
                close(fd[1]);
            set_st(0);
			break;
		}
		if (ft_strcmp(input, delimiter) == 0)
        {
            free(input);
            if (is_last_stdin == 0)
                close(fd[1]);
            set_st(0);
            break;
        }
        if (is_last_stdin == 0)
        {
            if (token[0]->tmp_value == DELIMITER_NO_QUOTE)
            {
                char *expanded = expand_heredoc_pipe(input, token[0]->env_lst);
                free(input);
                input = expanded;
            }
		    ft_putendl_fd(input, fd[1]);
        }
		free(input);
    }
}

void	execute_pipe_heredoc(t_token **token, int *pipe_heredoc, int len, int len_heredoc)
{
	int	heredoc_index = 0;
	int	i = 0;
	char	*delimiter;

	while (i < len)
	{
		if (token[i]->type == REDIR_HEREDOC && token[i + 1]->type == DELIMITER)
		{
            printf("delimiter [%s]\n", token[i + 1]->value);
            delimiter = token[i + 1]->value;
            int k = i + 1;
            int is_last_stdin = 0;
            while (k < len)
            {
                if (token[k]->type == REDIR_HEREDOC || token[k]->type == REDIR_IN)
                    is_last_stdin = 1;
                k++;
            }
            if (heredoc_index + 1 == len_heredoc && is_last_stdin == 0)
            {
                pipe(pipe_heredoc);
            }
            token[0]->tmp_value = token[i + 1]->type_quote_delim;
            looop(delimiter, pipe_heredoc, is_last_stdin, token);
            if (heredoc_index + 1 == len_heredoc && is_last_stdin == 0)
            {
                token[0]->fd_heredoc = pipe_heredoc[0];
            }
            heredoc_index++;
		}
		i++;
	}
}