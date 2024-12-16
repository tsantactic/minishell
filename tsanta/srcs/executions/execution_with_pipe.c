/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_with_pipe.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tambinin <tambinin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 10:16:09 by sandriam          #+#    #+#             */
/*   Updated: 2024/12/14 15:57:26 by tambinin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <stdio.h>

void count_cmd_per_arg(t_cmd *cmd)
{
    size_t i = 0;
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
                if (cmd->commands_arg[i][j]->type == CMD)
                    cmd->commands_arg[i][0]->command_value = ft_strdup(cmd->commands_arg[i][j]->value); // command_value = "ls" commande pple
                len++;
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
                /* mbola tsy ampy anle IS_ENV : export lol="ls -a" -> $lol*/
                cmd->commands_arg[i][0]->tmp_cmd_arg[k++] = ft_strdup(cmd->commands_arg[i][j]->value); // tmp_cmd_arg = ["ls", "-l"] char **s_token
            }
            j++;
        }
        cmd->commands_arg[i][0]->tmp_cmd_arg[k] = NULL;
        i++;
    }
}

void print_commands(t_cmd *cmd)
{
    size_t i = 0;
    int j = 0;

    printf("Total pipes: %ld\n", cmd->nb_pipe);
    while (i <= cmd->nb_pipe)
    {
        j = 0;
        printf("%ld command: ", i);
        while (cmd->commands_arg[i][j] && cmd->commands_arg[i][j]->value)
        {
            printf("%s (type %d)", cmd->commands_arg[i][j]->value, cmd->commands_arg[i][j]->type);
            j++;
        }
        if (i == cmd->nb_pipe)
            printf("\nfd is stdout\n");    
        else
            printf("\nfd is stdin\n");
        i++;
    }
}

void free_commands(t_cmd *cmd)
{
    size_t i = 0;
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

void	handle_pipe(t_cmd *cmd, t_env **env)
{
	size_t	i;
	char	**envp;
	char	**args;
	int		fd[2 * cmd->nb_pipe];
	pid_t	pid;

	i = 0;
	// creer tous les pipes
	while (i < cmd->nb_pipe)
	{
		if (pipe(fd + i * 2) == -1)
		{
			perror("pipe");
			exit(EXIT_FAILURE);
		}
		i++;
	}
	// executer chaque cmd
	i = 0;
	while (i <= cmd->nb_pipe)
	{
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}
		if (pid == 0)
		{
			if (i > 0)
            {
                if (dup2(fd[(i - 1) * 2], STDIN_FILENO) == -1)
                {
                    perror("dup2 input");
                    exit(EXIT_FAILURE);
                }
            }
            
            if (i < cmd->nb_pipe) {
                if (dup2(fd[i * 2 + 1], STDOUT_FILENO) == -1)
                {
                    perror("dup2 output");
                    exit(EXIT_FAILURE);
                }
            }
            parse_exec_redir_pipe(cmd->commands_arg[i], cmd->commands_arg[i][0]->total_len);
            
			// close all fd !used
			size_t	j = 0;
			while (j < 2 * cmd->nb_pipe)
			{
				close(fd[j]);
				j++;
			}
			args = cmd->commands_arg[i][0]->tmp_cmd_arg;
			if (cmd->commands_arg[i][0]->command_value)
			{
                // printf("1[%s]\n", cmd->commands_arg[i][0]->command_value);
                char *extract_command = extract_command_bin(cmd->commands_arg[i][0]->command_value);
                // printf("2[%s]\n", extract_command_bin(cmd->commands_arg[i][0]->command_value));
				if (is_builtin(extract_command))
                {
                    if (ft_strcmp(extract_command, "echo") == 0)
                        ft_echo(args, env);
                    if (ft_strcmp(extract_command, "pwd") == 0)
                        ft_pwd(args);
                    if (ft_strcmp(extract_command, "export") == 0)
                        ft_export(args, env);
                    if (ft_strcmp(extract_command, "env") == 0)
                        ft_env(args, env);
                    if (ft_strcmp(extract_command, "unset") == 0)
                        ft_unset(args, env);
                    if (ft_strcmp(extract_command, "cd") == 0)
                        ft_cd(args, env);
                    if (ft_strcmp(extract_command, "exit") == 0)
                        ft_exit(args);
					exit(set_st(-1));
                }
                else
                {
                    char	*path = ft_find_path(extract_command, env);
                    if (path)
                    {
			            envp = env_list_to_array(*env);
                        signal(SIGQUIT, sig_quit_slash);
                        if (execve(path, args, envp)==-1)
                        {
                            ft_putendl_fd(extract_command, 2);
                            ft_putendl_fd("minishell: command not found", STDERR_FILENO);
                            // perror("execve");
                            ft_free(envp);
                            free(path);
                            ft_free_token_cmd(cmd);
                            free_new_env(env);                        
                            free_commands(cmd);
                            free_tokens(cmd);
                            free(extract_command);
                            free(cmd->commands_arg);
                            exit(EXIT_FAILURE);
                        }
                    }
                    else
                    {
                        ft_putendl_fd(extract_command, 2);
                        ft_putendl_fd("minishell: command not found", STDERR_FILENO);
                        ft_free_token_cmd(cmd);
                        free_new_env(env);
                        free(extract_command);
                        free_commands(cmd);
                        free_tokens(cmd);
                        free(cmd->commands_arg);
                        exit(EXIT_FAILURE);
                    }   
                }
			}
            else
            {
                if (i > 0 && i < cmd->nb_pipe)
                {
                    close(fd[(i - 1) * 2 + 1]);
                }
                ft_free_token_cmd(cmd);
                free_new_env(env);

                free_commands(cmd);
                free_tokens(cmd);
                free(cmd->commands_arg);
                exit(EXIT_FAILURE);
            }

		}
		i++;
	}
	i = 0;
	while (i < 2 * cmd->nb_pipe)
	{
		close(fd[i]);
		i++;
	}
	i = 0;
	while (i <= cmd->nb_pipe)
	{
		wait(NULL);
		i++;
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
    // print_commands(cmd);
    count_cmd_per_arg(cmd);
	handle_pipe(cmd, env);
	// liberer les memoires
    free_commands(cmd);
    free(cmd->commands_arg);
}
