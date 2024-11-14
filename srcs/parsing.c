/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sandriam <sandriam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 15:36:25 by tambinin          #+#    #+#             */
/*   Updated: 2024/11/14 17:17:17 by sandriam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/**remove quote */
t_cmd	*move_quote(t_cmd *cmd, char **env)
{
	char	*cleaned_arg;

	cmd->is_empty = 0;
	cmd->index = 0;
	while (cmd->index < cmd->len_arg)
	{
		cleaned_arg = remove_quotes_and_expand(cmd,
				cmd->tokens[cmd->index]->value, env,
				cmd->tokens[cmd->index]->type);
		if (cmd->is_empty && cmd->index < cmd->len_arg && cmd->tokens[cmd->index
			- 1]->type == 2)
		{
			cmd->tokens[cmd->index]->type = 1;
			cmd->is_empty = 0;
		}
		if (cmd->tokens[cmd->index]->type == 1 && ft_strlen(cleaned_arg) == 0
			&& cmd->tokens[cmd->index]->in_quote == NO_QUOTE)
		{
			cmd->tokens[cmd->index]->type = 2;
			cmd->is_empty = 1;
		}
		free(cmd->tokens[cmd->index]->value);
		cmd->tokens[cmd->index]->value = cleaned_arg;
		cmd->index++;
	}
	return (cmd);
}
void	free_tokens(t_cmd *cmd)
{
	int	i;

	i = 0;
	while (i < cmd->len_tokens)
	{
		free(cmd->tokens[i]->value);
		free(cmd->tokens[i]);
		i++;
	}
	free(cmd->tokens);
}

void	count_pipe(t_cmd *cmd)
{
	int	i;

	i = 0;
	cmd->nb_pipe = 0;
	while (i < cmd->len_tokens)
	{
		if (cmd->tokens[i]->type == 4)
			cmd->nb_pipe += 1;
		i++;
	}
}

int	parse_cmd(char *line, t_cmd *cmd, char **env)
{
	int	i;
	int	j;

	cmd->tokens = NULL;
	cmd->len_tokens = 0;
	(void)cmd;
	i = 0;
	(void)env;
	i = count_arg(line, cmd);
	cmd->len_arg = i;
	if (i < 0)
	{
		if (i == -1)
			ft_putendl_fd("minishell: syntax error with unclosed quotes", 2);
		else if (i == -2)
			ft_putendl_fd("minishell: syntax error near unexpected token `|'", 2);
		return (-1);
	}
	else
	{
		stock_arg(line, cmd);
		// for (int i = 0; i < cmd->arg_index; i++)
		// {
		// 	printf("[%s]\n", cmd->args[i]);
		// }
		// printf("/*******step 1 separate with space******/\n");
		cmd->token_arg = malloc(sizeof(char *) * (cmd->len_arg + 1));
		if (!cmd->token_arg)
		{
			return (-1);
		}
		stock_token(cmd);
		// i = 0;
		// while (i < cmd->len_arg)
		// {
		// 	printf("[%s]\n", cmd->token_arg[i]);
		// 	i++;
		// }
		// printf("/******step 2 type*******/\n");
		type_token(cmd);
		// for (int i = 0; i < cmd->len_tokens; i++)
		// {
		// 	printf("Token Value: [%s], Type: %d\n", cmd->tokens[i]->value,
		// 		cmd->tokens[i]->type);
		// }
		// printf("/******step 4 remove and expansion*******/\n");
		cmd = move_quote(cmd, env);
		for (int i = 0; i < cmd->len_tokens; i++)
		{
			printf("Token Value: [%s], Type: %d\n", cmd->tokens[i]->value,
				cmd->tokens[i]->type);
		}
		/*check syntax error*/
		if (check_error_arg(cmd->tokens, cmd->len_tokens) > 0)
		{
			/*execute command*/
			count_pipe(cmd);
			// printf("\nnous avons %ld pipe\n", cmd->nb_pipe);
			if (cmd->nb_pipe == 0 && cmd->len_arg > 0) /*tsisy pipe*/
				fork_exec(cmd, env);
			else if (cmd->nb_pipe > 0 && cmd->len_arg > 0) /*misy pipe*/
				execute_with_pipes(cmd, env);
			return (0);
		}
		else
		{
			j = 0;
			while (j < cmd->len_arg)
			{
				free(cmd->token_arg[j]);
				j++;
			}
			free(cmd->token_arg);
			cmd->token_arg = NULL;
			free_tokens(cmd);
			j = 0;
			while (cmd->args[j] != NULL)
				free(cmd->args[j++]);
			return (-1);
		}
	}
	return (0);
}
