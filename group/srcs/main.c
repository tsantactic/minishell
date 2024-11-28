/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sandriam <sandriam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 08:37:25 by sandriam          #+#    #+#             */
/*   Updated: 2024/11/28 15:41:14 by sandriam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void loop_readline(t_cmd *cmd, char **env)
{
    char *input;
    while (1)
	{
		input = readline("$tambinin&&sandriam@minishell%> ");
		if (input == NULL)
		{
			printf("exit\n");
			return ;
		}
		if (ft_strlen(input) == 0)
		{
			free(input);
			continue ;
		}
		add_history(input);
		if (strncmp(input, "exit", 5) == 0)
		{
			rl_clear_history();
			printf("exit\n");
			free(input);
			return ;
		}
        int tmp = lexing_arg(input, cmd, env);
		if (tmp == 0)
		{
            printf("compilation...\n");
            exec_alls(cmd, env); /******* execution with or without pipe *****/
            free_tokens_all(cmd);
		}
        else if (tmp == -1)
            free_and_reinitialise(cmd);
		free(input);
	}
}
int set_st(int nb)
{
	static int i;
	if (nb >= 0)
		i = nb;
	return (i);
}
int main(int arg, char **argv, char **env)
{
    t_cmd	cmd;

	(void)arg;
	(void)argv;
    signal(SIGINT, sig_handler);
	signal(SIGQUIT, SIG_IGN);
	set_st(-1);
	cmd.args = malloc(sizeof(char *) * INITIAL_ARG_SIZE);
	if (!cmd.args)
	{
		printf("Failed to allocate memory for cmd.args\n");
		return (EXIT_FAILURE);
	}
	memset(cmd.args, 0, sizeof(char *) * INITIAL_ARG_SIZE);
    loop_readline(&cmd, env);
    free(cmd.args);
    rl_clear_history();
    return (set_st(-1));
}
