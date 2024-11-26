/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executions.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sandriam <sandriam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 14:34:15 by sandriam          #+#    #+#             */
/*   Updated: 2024/11/26 11:56:14 by sandriam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTIONS_H
# define EXECUTIONS_H

typedef struct s_cmd t_cmd;
void     execute_with_pipes(t_cmd *cmd, char **env);
void    execute_without_pipe(t_cmd *cmd, char **env);
void parsing_argument_simple(t_cmd *cmd, char **env);
void parsing_arg_with_builtins(t_cmd *cmd, char **env);
void loop_heredoc(char *delimiter, int *pipefd);
const char	*get_env_value(const char *var, char **env);
void	ft_perror(char *msg);
void	count_pipe(t_cmd *cmd);
void    exec_alls(t_cmd *cmd, char **env);
char	*ft_is_path(char **envp);
char	*build_path_t_cmd(char *path, char *t_cmd);
char	*ft_find_path(char *t_cmd, char **envp);
void	ft_execute_command(t_cmd *cmd, char *path, char **my_t_cmd, char **envp);
void    redir_exec(t_cmd *cmd, char **envp);
void redir_exec_without_buitin(t_cmd *cmd, char **envp);
char    *copy_command_arg(t_cmd *cmd, char **my_t_cmd, char *command);

int contains_bin(const char *command);
void free_and_reinitialise(t_cmd *cmd);

/*parse and exec simple arg*/
int count_heredoc_arg(t_cmd *cmd, int count_heredoc);
void loop_heredoc(char *delimiter, int *pipefd);
void execute_heredoc(t_cmd *cmd, int **pipe_heredoc, int count_heredoc);
int contains_redirection(t_cmd *cmd);
int contains_heredoc(t_cmd *cmd);
void count_command_arg(t_cmd *cmd, int *len_command);
char  *copy_command_arg(t_cmd *cmd, char **my_t_cmd, char *command);
int redirection_exec(t_cmd *cmd);
int contains_bin(const char *command);
char *extract_command_bin(const char *command);
int is_builtin(char *command);
#endif