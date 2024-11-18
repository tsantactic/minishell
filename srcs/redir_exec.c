/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sandriam <sandriam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 13:44:18 by sandriam          #+#    #+#             */
/*   Updated: 2024/11/18 18:50:20 by sandriam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
/*cmd->len_tokens, cmd->tokens[i]->value, cmd->tokens[i]->type*/
/*et */
/*d'utiliser char **args dans le structure de cmd : cmd->args*/
/*ls -a << e > a*/
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
    for (int i = 0; i < cmd->len_tokens; i++)
        if (cmd->tokens[i]->type == REDIR_HEREDOC)
            count_heredoc++;

    pipe_heredoc = malloc(sizeof(int *) * count_heredoc);
    for (int i = 0; i < count_heredoc; i++)
        pipe_heredoc[i] = malloc(sizeof(int) * 2);

    int heredoc_index = 0;

    while (i < cmd->len_tokens)
    {  
        if (cmd->tokens[i]->type == CMD || cmd->tokens[i]->type == ARG)
            len_command++;
        if (cmd->tokens[i]->type == REDIR_HEREDOC)
        {
            char *delimiter = cmd->tokens[i + 1]->value;
            pipe(pipe_heredoc[heredoc_index]);
            loop_heredoc(delimiter, pipe_heredoc[heredoc_index]);
            heredoc_index++;
            i += 1;
        }
        i++;
    }
    if (count_heredoc > 0)
    {
        dup2(pipe_heredoc[count_heredoc - 1][0], STDIN_FILENO);
        close(pipe_heredoc[count_heredoc - 1][0]);
        close(pipe_heredoc[count_heredoc - 1][1]);
    }
    my_t_cmd = malloc(sizeof(char *) * (len_command + 1));
    if (!my_t_cmd)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    command = copy_command_arg(cmd, my_t_cmd, command);
    int k = 0;
    while (k < cmd->len_tokens)
    {
        if (cmd->tokens[k]->type == REDIR_OUT)
        {
            int fd = open(cmd->tokens[k + 1]->value, O_WRONLY | O_CREAT | O_TRUNC, 0777);
            if (fd < 0)
            {
                perror(cmd->tokens[k + 1]->value);
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
        path = ft_find_path(command, envp);
        ft_execute_command(cmd, path, my_t_cmd, envp);
        free(command);
        free(path);
    }
    free_token_list(cmd);
    ft_free(cmd->args);
    ft_free(my_t_cmd);
    exit(0);
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
