/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sandriam <sandriam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 13:44:18 by sandriam          #+#    #+#             */
/*   Updated: 2024/11/19 14:59:19 by sandriam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
/*cmd->len_tokens, cmd->tokens[i]->value, cmd->tokens[i]->type*/
/*et */
/*d'utiliser char **args dans le structure de cmd : cmd->args*/
/*ls -a << e > a*/
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

void execute_heredoc(t_cmd *cmd, int **pipe_heredoc, int *len_command, int count_heredoc, int *herdoc_fd)
{
    int heredoc_index = 0;
    int i;
    char *delimiter;

    delimiter = NULL;
    i = 0;
    while (i < cmd->len_tokens)
    {
        if (cmd->tokens[i]->type == CMD || cmd->tokens[i]->type == ARG)
            (*len_command)++;
        if (cmd->tokens[i]->type == REDIR_HEREDOC)
        {
            delimiter = cmd->tokens[i + 1]->value;
            pipe(pipe_heredoc[heredoc_index]);
            loop_heredoc(delimiter, pipe_heredoc[heredoc_index]);
            if (heredoc_index + 1 == count_heredoc)
            {
                *herdoc_fd = pipe_heredoc[heredoc_index][0];
                close(pipe_heredoc[heredoc_index][1]);
            }
            else
                close(pipe_heredoc[heredoc_index][0]);
            heredoc_index++;
            i += 1;
        }
        i++;
    }
}
void redir_exec(t_cmd *cmd, char **envp)
{
    int i = 0;
    char **my_t_cmd = NULL;
    int len_command = 0;
    char *path = NULL;
    char *command = NULL;
    i = 0;
    int **pipe_heredoc;
    int count_heredoc = 0;
    int heredoc_fd = -1;
    count_heredoc = count_heredoc_arg(cmd, count_heredoc);
    pipe_heredoc = malloc(sizeof(int *) * count_heredoc);
    i = 0;
    while (i < count_heredoc)
        pipe_heredoc[i++] = malloc(sizeof(int) * 2);

    execute_heredoc(cmd, pipe_heredoc, &len_command, count_heredoc, &heredoc_fd);
    my_t_cmd = malloc(sizeof(char *) * (len_command + 1));
    if (!my_t_cmd)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    command = copy_command_arg(cmd, my_t_cmd, command);
    int k = 0;
    if (heredoc_fd != -1 && ft_strcmp(command, "cd") != 0)
    {
        dup2(heredoc_fd, STDIN_FILENO);
        close(heredoc_fd);
    }
    while (k < cmd->len_tokens)
    {
        if (cmd->tokens[k]->type == REDIR_OUT)
        {
            int fd = open(cmd->tokens[k + 1]->value, O_WRONLY | O_CREAT | O_TRUNC, 0777);
            if (fd < 0)
            {
                perror(cmd->tokens[k + 1]->value);
                free_token_list(cmd);
                ft_free(cmd->args);
                ft_free(my_t_cmd);
                i = 0;
                while (i < count_heredoc)
                {
                    free(pipe_heredoc[i]);
                    i++;
                }
                free(pipe_heredoc);
                exit(EXIT_FAILURE);
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
                perror(cmd->tokens[k + 1]->value);
                free_token_list(cmd);
                ft_free(cmd->args);
                ft_free(my_t_cmd);
                i = 0;
                while (i < count_heredoc)
                {
                    free(pipe_heredoc[i]);
                    i++;
                }
                free(pipe_heredoc);
                free(command);
                exit(EXIT_FAILURE);
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
                perror(cmd->tokens[k + 1]->value);
                free_token_list(cmd);
                ft_free(cmd->args);
                ft_free(my_t_cmd);
                i = 0;
                while (i < count_heredoc)
                {
                    free(pipe_heredoc[i]);
                    i++;
                }
                free(pipe_heredoc);
                exit(EXIT_FAILURE);
            }
            dup2(fd, 1);
            close(fd);
            k++;
        }
        k++;
    }
    if (command)
    {
        i = 0;
        while (i < count_heredoc)
        {
            free(pipe_heredoc[i]);
            i++;
        }
        free(pipe_heredoc);
        if (is_builtin(command) == 0)
        {
            path = ft_find_path(command, envp);
            ft_execute_command(cmd, path, my_t_cmd, envp);
            free(command);
            free(path);
        }
        else
        {
            if (ft_strcmp(command, "cd") == 0)
            {
                if (my_t_cmd[1] != NULL)
                {
                    if (chdir(my_t_cmd[1]) == -1)
                    {
                        perror("cd");
                    }
                }
                else
                {
                    ft_putendl_fd("without argument",2);
                }
            }
            free_token_list(cmd);
            ft_free(my_t_cmd);
            return ;
        }
    }
    free_token_list(cmd);
    ft_free(cmd->args);
    ft_free(my_t_cmd);
    i = 0;
    while (i < count_heredoc)
    {
        free(pipe_heredoc[i]);
        i++;
    }
    free(pipe_heredoc);
    return ;
}

/*ls -a << e > a.txt << b*/
/*priorite heredoc, donc classer tous les hererdoc avec ses delimiter et axecuter tant un par un le readline*/
/*recuperation de command de type 1 avec son argument de tous type 3 dans my_t_cmd*/
/*si les hereodc sont tous executer on executer les commandes et ses arguments si il y a*/
/*apres on execute les autres redirections si on a comme < , > , >>*/
/*voici les exemples dans cmd:*/
/*ls <<e -a  l execution: recuperation de ls -a dans my_t_cmd et attend la fin de l execution de heredoc e*/
/*ls -a << e meme cas re recuperation de ls -a ls de type 1 et -a de type 3 recuperer tous ses type 3 de ls de type 1*/
/*<<e<<a<<a ls execution des 3 heredoc car heredoc est priorite , apres verifier si ls est de type 1 on l execute*/
/*<<e<<a<<a ls -a meme cas mais ls a un option de type 3 on met dans my_t_cmd et execute si tous les heredoc sont executer un par un*/
/*ls > txt << o on execute le heredoc de o , apres on execute ls et met la sortie dasn txt*/
/*meme cas pour le <, >> , >, mais heredoc est priorite si il y a heredoc*/
/*si il y a des type 1 et type 3 on l ensemble dans my_t_cmd et execute apres l execution de tous les heredoc*/
