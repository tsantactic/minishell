/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_normal.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sandriam <sandriam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 15:15:05 by sandriam          #+#    #+#             */
/*   Updated: 2024/11/13 09:18:52 by sandriam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
/*pipe normal*/
void fork_exec(t_cmd *cmd, char *env[])
{
    pid_t pid;

    pid = fork();
    if (pid == -1)
        ft_perror("fork");
    if (pid == 0)
    {
        execute_all(cmd, env);
		exit(0);
    }
    else
    {
        if (waitpid(pid, NULL, 0) == -1)
            ft_perror("waitpid");
		free_token_list(cmd);
    }
}
