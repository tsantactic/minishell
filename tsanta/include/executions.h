/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executions.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tambinin <tambinin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 14:34:15 by sandriam          #+#    #+#             */
/*   Updated: 2024/12/20 15:14:08 by tambinin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTIONS_H
# define EXECUTIONS_H
// # include "minishell.h"

typedef struct s_cmd	t_cmd;
typedef struct s_env	t_env;

void					exec_alls(t_cmd *cmd, t_env **envs);
void					execute_with_pipes(t_cmd *cmd, t_env **env);
void					execute_without_pipe(t_cmd *cmd, t_env **env);
void					parsing_argument_simple(t_cmd *cmd, t_env **env);
void					parsing_arg_with_builtins(t_cmd *cmd, t_env **env);
char					*expand_heredoc(char *input, t_cmd *cmd);
void					loop_heredoc_blt(char *delimiter, int *pipefd,
							t_cmd *cmd);

void					path_is_here(char *path, t_env **env, char **tmp_cmd,
							char *command, t_cmd *cmd);
void					path_not_set(char *command, t_env **env,
							char **tmp_cmd);

const char				*get_env_value(const char *var, t_env **env);
void					ft_perror(char *msg);
void					count_pipe(t_cmd *cmd);
char					*ft_is_path(t_env **envp);
char					*build_path_t_cmd(char *path, char *t_cmd);
char					*ft_find_path(char *t_cmd, t_env **envp);
char					*copy_command_arg(t_cmd *cmd, char **my_t_cmd,
							char *command);
int						check_env(t_env **envp, char *value);
char					*copy_command(t_cmd *cmd, char *command);
void					ft_execute_command(t_cmd *cmd, char *path,
							char **my_t_cmd, t_env **envp);
void					handle_stat_error(char **my_t_cmd, t_cmd *cmd);
void					handle_errno_case(char **my_t_cmd, t_cmd *cmd);
void					cleanup_resources(t_cmd *cmd, char **env);
void					execute_and_handle_error(t_cmd *cmd, char *path,
							char **my_t_cmd, char **env);
void					check_and_set_exit_status(char **my_t_cmd,
							int is_not_cmd, int is_not_dir);

void					execute_heredoc(t_cmd *cmd, int **pipe_heredoc,
							int count_heredoc);
int						count_heredoc_arg(t_cmd *cmd, int count_heredoc);
int						contains_redirection(t_cmd *cmd);
int						contains_heredoc(t_cmd *cmd);
void					loop_heredoc(char *delimiter, int *pipefd, t_cmd *cmd,
							int type_quote_delim);

void					count_command_arg(t_cmd *cmd, int *len_command);
char					*copy_command_arg(t_cmd *cmd, char **my_t_cmd,
							char *command);
void					count_command_arg_blt(t_cmd *cmd, int *len_command);
char					*copy_command_arg_blt(t_cmd *cmd, char **my_t_cmd,
							char *command);

int						redirection_exec(t_cmd *cmd);
int						contains_bin(char *command);
char					*extract_command_bin(char *command);
int						is_builtin(char *command);

void					parse_exec_heredoc(t_cmd *cmd);
void					parse_exec_redirection(t_cmd *cmd);

void					free_and_reinitialise(t_cmd *cmd);

void					ft_cd(char **args, t_env **envp);
void					ft_echo(char **args, t_env **env);
void					ft_pwd(char **argv);
void					ft_env(char **argv, t_env **env);
// void ft_exit(char **args);
void					ft_exit_pipe(char **args, t_cmd *cmd, t_env **env,
							char *extract_cmd);
void					ft_exit_simple(t_cmd *cmd, char **args, t_env **env);
int						is_valid_id(char *args);
void					ft_export(char **args, t_env **env);
void					update_or_add_variable(t_env **env, const char *var);
void					display_export(t_env *env);
void					update_env_var(const char *var_name, const char *value,
							t_env **env_list);
char					*ft_strjoin3(const char *s1, const char *s2,
							const char *s3);
void					unset_env_var(const char *var_name, t_env **env);
void					ft_unset(char **args, t_env **env);

#endif
