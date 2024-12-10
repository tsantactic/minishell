/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_arg_builtins.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sandriam <sandriam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 09:26:57 by sandriam          #+#    #+#             */
/*   Updated: 2024/12/06 17:02:21 by sandriam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
void loop_heredoc_blt(char *delimiter, int *pipefd, t_cmd *cmd)
{
    char *input = NULL;
    set_sig_heredoc(0);
    while (1)
    {
        signal(SIGINT, sig_handler_heredoc);
        input = readline("heredoc>");
        if (set_sig_heredoc(-1) == 1)
        {
            set_sig_new_line(1);
            if (input)
                free(input);
            close(pipefd[1]);
            break;
        }
        if (!input)
        {
            set_sig_new_line(1);
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
        (void)cmd;
        ft_putendl_fd(input, pipefd[1]);    
        free(input);
    }
}

void execute_heredoc_builtins(t_cmd *cmd, int **pipe_heredoc, int count_heredoc, int *temp_pipe_heredoc)
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
                loop_heredoc_blt(delimiter, pipe_heredoc[heredoc_index], cmd);
                if (set_sig_heredoc(-1) == 1)
                {
                    close(pipe_heredoc[heredoc_index][0]);
                    break;
                }
                if (heredoc_index + 1 == count_heredoc)
                {
                    //(void)*temp_pipe_heredoc;
                    *temp_pipe_heredoc = pipe_heredoc[heredoc_index][0];
                    //dup2(pipe_heredoc[heredoc_index][0], STDIN_FILENO);
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
void exec_heredoc_builtins(t_cmd *cmd, int temp_pipe_heredoc)
{
    if (contains_heredoc(cmd))
    {
        cmd->count_heredoc = count_heredoc_arg(cmd, cmd->count_heredoc);
        cmd->pipe_heredoc = malloc(sizeof(int *) * cmd->count_heredoc);
        int i = 0;
        while (i < cmd->count_heredoc)
            cmd->pipe_heredoc[i++] = malloc(sizeof(int) * 2);
        execute_heredoc_builtins(cmd, cmd->pipe_heredoc, cmd->count_heredoc, &temp_pipe_heredoc);
    }
}
int exec_redir_builtins(t_cmd *cmd)
{
   if (contains_redirection(cmd) && set_sig_heredoc(-1) != 1)   
    {
        if (redirection_exec(cmd) == 2)
        {
            if (contains_heredoc(cmd))
                free_pipe_heredoc(cmd->count_heredoc, cmd->pipe_heredoc);
            dup2(cmd->stdout_backup, STDOUT_FILENO);
            dup2(cmd->stdin_backup, STDIN_FILENO);
            close(cmd->stdout_backup);
            close(cmd->stdin_backup);
            return (1);
        }
    }
    return (0);
}
void parsing_arg_with_builtins(t_cmd *cmd, t_env **env)
{
    /*parse redirection heredoc*/
    int temp_pipe_heredoc = -1;
    cmd->pipe_heredoc = NULL;
    cmd->count_heredoc = 0;
    cmd->stdout_backup = dup(STDOUT_FILENO);
    cmd->stdin_backup = dup(STDIN_FILENO);
    exec_heredoc_builtins(cmd, temp_pipe_heredoc);
    /*parse redirection in , out*/
    if (exec_redir_builtins(cmd) == 1)
    {
        return ;
    }

    /*parse command*/
    char **tmp_cmd = NULL;
    int len_command = 0;
    char *command = NULL;
    count_command_arg_blt(cmd, &len_command);
    tmp_cmd = malloc(sizeof(char *) * (len_command + 1));
    if (tmp_cmd == NULL)
    {
        if (contains_heredoc(cmd))
            free_pipe_heredoc(cmd->count_heredoc, cmd->pipe_heredoc);
        if (set_sig_heredoc(-1) == 1)
            set_sig_heredoc(0);
        return;
    }
    command = copy_command_arg_blt(cmd, tmp_cmd, command);
    if (command)
    {
        char *extracted_command = extract_command_bin(command);
        if (is_builtin(extracted_command))
        {
            (void)env;
            free(tmp_cmd[0]);
            tmp_cmd[0] = extracted_command;
            free(command);
            if (ft_strcmp(tmp_cmd[0], "cd") == 0 && set_sig_heredoc(-1) != 1)
                ft_cd(tmp_cmd, env);
            if (ft_strcmp(tmp_cmd[0], "pwd") == 0 && set_sig_heredoc(-1) != 1)
                ft_pwd(tmp_cmd);
            if (ft_strcmp(tmp_cmd[0], "echo") == 0 && set_sig_heredoc(-1) != 1)
                ft_echo(tmp_cmd, env);
            if (ft_strcmp(tmp_cmd[0], "env") == 0 && set_sig_heredoc(-1) != 1)
                ft_env(tmp_cmd, env);
            if (ft_strcmp(tmp_cmd[0], "export") == 0 && set_sig_heredoc(-1) != 1)
                ft_export(tmp_cmd, env);
            if (ft_strcmp(tmp_cmd[0], "exit") == 0 && set_sig_heredoc(-1) != 1)
            {
                ft_exit(tmp_cmd);
                if (set_st(-1) != 1)
                {
                    dup2(cmd->stdout_backup, STDOUT_FILENO);
                    dup2(cmd->stdin_backup, STDIN_FILENO);
                    close(cmd->stdout_backup);
                    close(cmd->stdin_backup);
                    if (tmp_cmd)
                        ft_free(tmp_cmd);
                    if (contains_heredoc(cmd))
                        free_pipe_heredoc(cmd->count_heredoc, cmd->pipe_heredoc);
                    ft_free_token_cmd(cmd);
                    free_new_env(env);
                    free_tokens(cmd);
                    exit(set_st(-1));
                }
            }
            if (ft_strcmp(tmp_cmd[0], "unset") == 0 && set_sig_heredoc(-1) != 1)
                ft_unset(tmp_cmd, env);
            if (set_sig_heredoc(-1) == 1)
            {
                set_sig_heredoc(0);
                write(1, "\n", 1);
            }
            dup2(cmd->stdout_backup, STDOUT_FILENO);
            dup2(cmd->stdin_backup, STDIN_FILENO);
            close(cmd->stdout_backup);
            close(cmd->stdin_backup);
            if (tmp_cmd)
                ft_free(tmp_cmd);
            if (contains_heredoc(cmd))
                free_pipe_heredoc(cmd->count_heredoc, cmd->pipe_heredoc);
            return ;
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
        set_st(0);
        return;
    }
}
