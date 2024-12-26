/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_arg_builtins_two.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sandriam <sandriam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 09:00:22 by tambinin          #+#    #+#             */
/*   Updated: 2024/12/24 12:01:23 by sandriam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	handle_builtins_simple(char **tmp_cmd, t_env **env)
{
	if (ft_strcmp(tmp_cmd[0], "cd") == 0)
		ft_cd(tmp_cmd, env);
	if (ft_strcmp(tmp_cmd[0], "pwd") == 0)
		ft_pwd(tmp_cmd);
	if (ft_strcmp(tmp_cmd[0], "echo") == 0)
		ft_echo(tmp_cmd);
	if (ft_strcmp(tmp_cmd[0], "env") == 0)
		ft_env(tmp_cmd, env);
	if (ft_strcmp(tmp_cmd[0], "export") == 0)
		ft_export(tmp_cmd, env);
	if (ft_strcmp(tmp_cmd[0], "unset") == 0)
		ft_unset(tmp_cmd, env);
}

void	handle_exit_builtins(char **tmp_cmd, t_cmd *cmd, t_env **env)
{
	if (ft_strcmp(tmp_cmd[0], "exit") == 0)
	{
		ft_exit_simple(cmd, tmp_cmd, env);
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
}

void	execute_builtins(char **tmp_cmd, t_cmd *cmd, char *command, t_env **env)
{
	char	*extracted_command;

	extracted_command = extract_command_bin(command);
	if (is_builtin(extracted_command))
	{
		free(tmp_cmd[0]);
		tmp_cmd[0] = extracted_command;
		free(command);
		if (set_sig_heredoc(-1) != 1)
			handle_builtins_simple(tmp_cmd, env);
		if (set_sig_heredoc(-1) != 1)
			handle_exit_builtins(tmp_cmd, cmd, env);
		if (set_sig_heredoc(-1) == 1)
		{
			set_sig_heredoc(0);
			write(1, "\n", 1);
		}
		reset_after_builtin(cmd, tmp_cmd);
		return ;
	}
	free(command);
	free(extracted_command);
	if (contains_heredoc(cmd))
		free_pipe_heredoc(cmd->count_heredoc, cmd->pipe_heredoc);
}

void	parsing_arg_with_builtins(t_cmd *cmd, t_env **env)
{
	char	**tmp_cmd;
	int		len_command;
	char	*command;

	init_parsing(cmd);
	if (exec_redir_builtins(cmd) == 1)
		return ;
	tmp_cmd = NULL;
	len_command = 0;
	command = NULL;
	count_command_arg_blt(cmd, &len_command);
	tmp_cmd = malloc(sizeof(char *) * (len_command + 1));
	if (tmp_cmd == NULL)
	{
		if (contains_heredoc(cmd))
			free_pipe_heredoc(cmd->count_heredoc, cmd->pipe_heredoc);
		if (set_sig_heredoc(-1) == 1)
			set_sig_heredoc(0);
		return ;
	}
	command = copy_command_arg_blt(cmd, tmp_cmd, command);
	if (command)
		execute_builtins(tmp_cmd, cmd, command, env);
	else
		clean_memory(cmd, tmp_cmd);
}
