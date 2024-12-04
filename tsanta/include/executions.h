/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executions.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sandriam <sandriam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 14:34:15 by sandriam          #+#    #+#             */
/*   Updated: 2024/12/03 17:56:56 by sandriam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTIONS_H
# define EXECUTIONS_H

typedef struct s_cmd t_cmd;
typedef struct s_env t_env;
void    exec_alls(t_cmd *cmd, t_env **env);
void     execute_with_pipes(t_cmd *cmd, t_env **env);
void    execute_without_pipe(t_cmd *cmd, t_env **env);
void parsing_argument_simple(t_cmd *cmd, t_env **env);
void parsing_arg_with_builtins(t_cmd *cmd, t_env **env);
char *expand_heredoc(char *input, t_cmd *cmd);
void loop_heredoc_blt(char *delimiter, int *pipefd, t_cmd *cmd);

const char	*get_env_value(const char *var, t_env **env);
void	ft_perror(char *msg);
void	count_pipe(t_cmd *cmd);
char	*ft_is_path(t_env **envp);
char	*build_path_t_cmd(char *path, char *t_cmd);
char	*ft_find_path(char *t_cmd, t_env **envp);
void	ft_execute_command(t_cmd *cmd, char *path, char **my_t_cmd, t_env **envp);
char    *copy_command_arg(t_cmd *cmd, char **my_t_cmd, char *command);


/*parse and exec simple arg*/
void execute_heredoc(t_cmd *cmd, int **pipe_heredoc, int count_heredoc);
int count_heredoc_arg(t_cmd *cmd, int count_heredoc);
void loop_heredoc(char *delimiter, int *pipefd, t_cmd *cmd);
int contains_redirection(t_cmd *cmd);
int contains_heredoc(t_cmd *cmd);
void count_command_arg(t_cmd *cmd, int *len_command);
char  *copy_command_arg(t_cmd *cmd, char **my_t_cmd, char *command);
int redirection_exec(t_cmd *cmd);
int contains_bin(const char *command);
char *extract_command_bin(const char *command);
int is_builtin(char *command);

void parse_exec_heredoc(t_cmd *cmd);
void parse_exec_redirection(t_cmd *cmd);

void free_and_reinitialise(t_cmd *cmd);
#endif