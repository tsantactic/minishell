/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executions.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sandriam <sandriam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 14:34:15 by sandriam          #+#    #+#             */
/*   Updated: 2024/12/26 13:53:48 by sandriam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTIONS_H
# define EXECUTIONS_H

typedef struct s_cmd	t_cmd;
typedef struct s_env	t_env;

void					exec_alls(t_cmd *cmd, t_env **envs);
void					execute_with_pipes(t_cmd *cmd, t_env **env);
void					execute_without_pipe(t_cmd *cmd, t_env **env);
void					process_execute_cmd(int i, t_cmd *cmd, t_env **env);
void					handle_pipe(t_cmd *cmd, t_env **env);
void					cleanup_parsing(t_cmd *cmd, char **tmp_cmd);
void					execute_with_stat(t_env **env, t_cmd *cmd,
							char **tmp_cmd);
void					execute_cmd_with_path(char *command, t_cmd *cmd,
							char **tmp_cmd, t_env **env);
void					handle_command_execution(t_cmd *cmd, char *command,
							char **tmp_cmd, t_env **env);
void					parsing_argument_simple(t_cmd *cmd, t_env **env);
void					parsing_arg_with_builtins(t_cmd *cmd, t_env **env);
char					*expand_heredoc(char *input, t_cmd *cmd);
void					loop_heredoc_blt(char *delimiter, int *pipefd);
void					init_parsing(t_cmd *cmd);
void					clean_memory(t_cmd *cmd, char **tmp_cmd);
void					reset_after_builtin(t_cmd *cmd, char **tmp_cmd);
void					execute_heredoc_builtins(t_cmd *cmd, int **pipe_heredoc,
							int count_heredoc, int *temp_pipe_heredoc);
int						exec_redir_builtins(t_cmd *cmd);
void					exec_heredoc_builtins(t_cmd *cmd,
							int temp_pipe_heredoc);

void					path_is_here(char *path, t_env **env, char *command,
							t_cmd *cmd);
void					path_not_set(char *command, t_env **env,
							char **tmp_cmd);
void					path_not_set_bin(char *command, t_env **env,
							char **tmp_cmd);
void					path_not_set_not_bin(char *command, t_env **env,
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
void					handle_stat_error(char **my_t_cmd, t_cmd *cmd,
							char **env);
void					handle_errno_case(char **my_t_cmd, t_cmd *cmd);
void					cleanup_resources(t_cmd *cmd, char **env);
void					execute_and_handle_error(t_cmd *cmd, char *path,
							char **my_t_cmd, char **env);
void					check_and_set_exit_status(char **my_t_cmd,
							int is_not_cmd, int is_not_dir, char **env);

int						check_if_stdin(int i, t_cmd *cmd);
void					manage_fd(int i, t_cmd *cmd, int **pipe_heredoc,
							int heredoc_index);
void					execute_heredoc_loop(t_cmd *cmd, int **pipe_heredoc,
							int heredoc_index, int i);
void					execute_heredoc(t_cmd *cmd, int **pipe_heredoc);
int						count_heredoc_arg(t_cmd *cmd, int count_heredoc);
int						contains_redirection(t_cmd *cmd);
int						contains_heredoc(t_cmd *cmd);
void					loop_heredoc(char *delimiter, int *pipefd, t_cmd *cmd,
							int type_quote_delim);
char					*expand_heredoc_pipe(char *input, t_env **env);
int						count_len_expand_value_pipe(t_env **env, char *input);

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
void					ft_echo(char **args);
void					ft_pwd(char **argv);
void					ft_env(char **argv, t_env **env);
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

void					handle_input_heredoc(char **input, char *delimiter);
int						is_delimiter_reached(char *input, char *delimiter);
void					handle_sigint(char *input);
void					loop_simple_heredoc(char *delimiter, t_cmd *cmd);
void					free_exit_simple(char **args, t_cmd *cmd, t_env **env,
							long exit_code);
void					free_exit_no_arg(t_cmd *cmd, char **args, t_env **env);

void					init_value_command(t_cmd *cmd, t_env **env);
void					free_exit_pipe(t_cmd *cmd, t_env **env);
void					free_commands(t_cmd *cmd);
void					execute_heredoc_pipe(t_cmd *cmd);
void					init_index(int *index);
void					init_command_allocation(t_cmd *cmd, int *index);
void					copy_token_to_command(t_cmd *cmd, int *index);
void					handle_pipe_and_finalize(t_cmd *cmd, int *index);
void					copy_value_command_arg(t_cmd *cmd);
char					*add_value(char *value, int *j, char **my_t_cmd,
							char *command);
int						count_env(char *value);
void					parse_token_cmd_pipe(int len, int i, int len_cmd,
							t_cmd *cmd);
void					exit_proccess_pipe(t_cmd *cmd);
void					init_execute_heredoc(t_cmd *cmd, t_env **env);
void					free_fd_pipe(int i, t_cmd *cmd);
void					init_fd_pipes(t_cmd *cmd);
void					init_pipes_and_pids(t_cmd *cmd);
void					close_fd_pipe(t_cmd *cmd);
void					close_fd_heredoc_possible(t_cmd *cmd);
void					free_fd_pipe_pid(t_cmd *cmd);
void					capture_status_command(t_cmd *cmd);
void					free_exit_processus_pipe(t_cmd *cmd, t_env **env);
void					init_stdin_pipe(int i, t_cmd *cmd);
void					init_stdin_heredoc_fd(int i, t_cmd *cmd);
void					exit_process_by_redir(t_cmd *cmd, t_env **env);
void					exit_by_not_found(t_cmd *cmd, t_env **env);
void					execute_command_exist(t_cmd *cmd, t_env **env, int i);
void					execute_builtins_cmd_pipe(int i, char *extract_command,
							t_cmd *cmd, t_env **env);
void					execute_builtins_pipe(int i, char *extract_command,
							t_env **env, t_cmd *cmd);
#endif
