/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tambinin <tambinin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 08:37:25 by sandriam          #+#    #+#             */
/*   Updated: 2024/12/17 17:03:17 by tambinin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	free_new_env(t_env **env)
{
	t_env	*curr;
	t_env	*new;

	curr = *env;
	while (curr)
	{
		free(curr->data);
		new = curr->next;
		free(curr);
		curr = new;
	}
	env = NULL;
}

void loop_readline(t_cmd *cmd, char **env)
{
    char *input;
	t_env *env_list;

	env_list = copy_env_to_list(env);
    while (1)
	{
		signal(SIGINT, sig_handler);
		signal(SIGQUIT, SIG_IGN);
		input = readline("$tambinin&&sandriam@minishell%> ");
		if (input == NULL)
		{
			free_new_env(&env_list);
			printf("exit\n");
			return ;
		}
		if (ft_strlen(input) == 0)
		{
			free(input);
			continue ;
		}
		add_history(input);
		if (ft_strncmp(input, "exit", 5) == 0)
		{
			free_new_env(&env_list);
			rl_clear_history();
			ft_putstr_fd("exit\n", 2);
			free(input);
			return ;
		}
        int tmp = lexing_arg(input, cmd, &env_list);
		if (tmp == 0)
		{
            exec_alls(cmd, &env_list); /******* execution with or without pipe *****/
			free_tokens_all(cmd);
		}
        else if (tmp == -1)
            free_and_reinitialise(cmd);
		free(input);
	}
}
int set_st(int nb)
{
	static int status;
	if (nb >= 0)
		status = nb;
	return (status);
}
int main(int arg, char **argv, char **env)
{
    t_cmd	cmd;

	(void)arg;
	(void)argv;
    
	set_st(-1);
	cmd.args = malloc(sizeof(char *) * INITIAL_ARG_SIZE);
	if (!cmd.args)
	{
		printf("Failed to allocate memory for cmd.args\n");
		return (EXIT_FAILURE);
	}
	ft_memset(cmd.args, 0, sizeof(char *) * INITIAL_ARG_SIZE);
    loop_readline(&cmd, env);
    free(cmd.args);
    rl_clear_history();
    return (set_st(-1));
}
