/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sandriam <sandriam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 16:40:52 by sandriam          #+#    #+#             */
/*   Updated: 2024/11/19 10:29:50 by sandriam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	sig_handler(int signum)
{
	if (signum == SIGINT)
	{
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}

void	sig_quit_handler(int signum)
{
	(void)signum;
}

int	main(int arg, char **argv, char **env)
{
	t_cmd	cmd;
	char	*input;
	int		i;

	(void)arg;
	(void)argv;
	i = 0;
	signal(SIGINT, sig_handler);
	signal(SIGQUIT, SIG_IGN);
	cmd.args = malloc(sizeof(char *) * INITIAL_ARG_SIZE);
	if (!cmd.args)
	{
		printf("Failed to allocate memory for cmd.args\n");
		return (EXIT_FAILURE);
	}
	memset(cmd.args, 0, sizeof(char *) * INITIAL_ARG_SIZE);
	while (1)
	{
		input = readline("$tambinin&&sandriam@minishell%> ");
		if (input == NULL)
		{
			printf("exit\n");
			break ;
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
			break ;
		}
		if (parse_cmd(input, &cmd, env) != -1)
		{
			i = 0;
			while (cmd.args[i] != NULL)
				free(cmd.args[i++]);
			memset(cmd.args, 0, sizeof(char *) * INITIAL_ARG_SIZE);
			cmd.arg_index = 0;
		}
		free(input);
	}
	free(cmd.args);
	rl_clear_history();
	return (0);
}
