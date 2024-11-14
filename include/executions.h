/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executions.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sandriam <sandriam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 14:34:15 by sandriam          #+#    #+#             */
/*   Updated: 2024/11/13 10:21:51 by sandriam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTIONS_H
# define EXECUTIONS_H

typedef struct s_cmd t_cmd;

void	execute_all(t_cmd *cmd, char *envp[]);
char	*ft_is_path(char **envp);
void	ft_perror(char *msg);
char	*build_path_t_cmd(char *path, char *t_cmd);
char	*ft_find_path(char *t_cmd, char **envp);
void	ft_execute_command(t_cmd *cmd, char *path, char **my_t_cmd, char **envp);
void    fork_exec(t_cmd *cmd, char *envp[]);
void     execute_with_pipes(t_cmd *cmd, char *env[]);
void redir_exec(t_cmd *cmd, char **envp);
#endif