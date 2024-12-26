/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sandriam <sandriam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 16:41:01 by sandriam          #+#    #+#             */
/*   Updated: 2024/12/24 17:41:55 by sandriam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include "executions.h"
# include "tokens.h"
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/ioctl.h>
# include <sys/resource.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <term.h>
# include <termios.h>
# include <unistd.h>

# define INITIAL_ARG_SIZE 100

typedef enum e_token_type
{
	CMD = 1,
	EMPTY,
	ARG,
	PIPE,
	REDIR_IN,
	INPUT_FILE,
	REDIR_OUT,
	OUTPUT_FILE,
	REDIR_OUTPUT_APPEND,
	OUTPUT_FILE_APPEND,
	REDIR_HEREDOC,
	DELIMITER,
	NO_QUOTE,
	IN_QUOTE,
	SIMPLE,
	IS_ENV,
	DELIMITER_WITH_QUOTE,
	DELIMITER_NO_QUOTE
}					t_token_type;

typedef struct s_token
{
	char			*value;
	t_env			**env_lst;
	int				tmp_value;
	int				len_cmd_arg;
	int				total_len;
	char			*tp_val;
	char			**tp_args;
	int				pipe_heredoc[2];
	int				count_heredoc;
	int				fd_heredoc;

	int				stdin_set;
	int				stdout_set;

	t_token_type	type;
	t_token_type	type_env;
	t_token_type	in_quote;
	t_token_type	type_delim;
}					t_token;

typedef struct s_env
{
	char			*data;
	struct s_env	*next;
}					t_env;

typedef struct s_cmd
{
	t_token			**tokens;
	t_token			***tp_arg;
	t_token_type	type;
	t_token_type	type_token;
	t_env			**env_lst;
	char			*cmd;
	char			**token_arg;
	char			**args;
	char			**lst_env;
	char			*path_cmd;
	char			**tmp_cmd;
	char			*extracted_command;
	int				len_tokens;
	int				len_arg;
	char			*tmp_value;
	int				type_del;
	int				len_cmd;

	pid_t			*pid;
	int				**pipe_heredoc;
	int				**fd_pipe;
	int				tmp_pipe_heredoc;
	int				count_heredoc;
	int				stdout_backup;
	int				stdin_backup;
	int				stdout_heredoc;
	int				stdin_heredoc;
	int				index_exit;
	int				in_single_quote;
	int				in_double_quote;
	int				in_word;
	int				redirect_count;
	int				var_start;
	int				start;
	int				arg_index;
	int				nb_pipe;
	int				state;
	int				is_cmd;
	int				infile;
	int				outfile;
	int				outapp;
	int				heredoc;
	int				index;
	int				is_empty;
	int				index_command;

	int				flag_exit;

}					t_cmd;

void				ft_free(char **str);
void				ft_free_dp(char **str, int len);
char				*ft_strndup(const char *s, int n);
char				*ft_strjoin_copy(char const *s1, char const *s2);
char				*count_len_cleaned(t_cmd *cmd, const char *str, char **env,
						t_token_type type);
void				loop_readline(t_cmd *cmd, char **env);
int					lexing_arg(char *line, t_cmd *cmd, t_env **env);
void				stock_token(t_cmd *cmd);
int					count_arg(char *s, t_cmd *cmd);
void				stock_arg(char *s, t_cmd *cmd);
char				*remove_quotes_and_expand(t_cmd *cmd, const char *str,
						t_env **env_list, t_token_type type);
void				toggle_quotes(t_cmd *cmd, char ch);
int					check_error_arg(t_token **tokens, int len_tokens);
int					set_index_syntax(t_token **tokens, int len_tokens);

void				sig_handler(int signum);
void				sig_quit_handler(int signum);
void				sig_quit_slash(int signum);
int					set_st(int state);
int					set_sig_heredoc(int st_heredoc);
void				sig_handler_heredoc(int signal);
void				sig_handler_heredoc_blt(int signal);
int					set_sig_new_line(int st_line);
void				sig_handler_heredoc_pipe(int signal);
void				add_env_node(t_env **head, char *value);
void				free_env_list(t_env *lst);
t_env				*copy_env_to_list(char **env);
char				**env_list_to_array(t_env *lst);
void				free_new_env(t_env **env);

int					parse_exec_redir_pipe(t_token **tp_arg, int len);
int					contains_redir_pipe(t_token **token, int len);
int					redirection_exec_pipe(t_token **tokens, int len);

void				parse_exec_heredoc_pipe(t_token **tp_arg, int len);
int					contains_heredoc_pipe(t_token **token, int len);
int					count_heredoc_pipe(t_token **token, int count_heredoc,
						int len);
void				execute_pipe_heredoc(t_token **token, int *pipe_heredoc,
						int len, int len_heredoc);

int					add_stdin(t_token **token, int len);
void				check_stdin(t_cmd *cmd);
int					add_stdout(t_token **token, int len);
void				check_stdout(t_cmd *cmd);

void				initialise_all(t_cmd *cmd, int i);
void				check_quote_delimiter(char *value,
						t_token_type *type_quote_delim);
void				last_type(t_cmd *cmd);
t_cmd				*handle_empty_or_quote(t_cmd *cmd, char *cleaned_arg,
						int index);
void				process_token_quote(t_cmd *cmd, t_env *env_list, int index);
t_cmd				*move_quote(t_cmd *cmd, t_env *env_list);
int					handle_lexing_error(int error_code);
int					process_tokens(t_cmd *cmd, t_env *env);

void				toggle_quote(char c, t_cmd *cmd);
int					handle_redirects(char *s, int *i);

void				toggle_quotes(t_cmd *cmd, char ch);
void				expand_status(size_t *indices, char *result);
void				init_all(t_cmd *cmd, size_t *indices);
void				add_double_quote(const char *str, size_t *indices,
						char *result, t_cmd *cmd);
void				skip_add_value(t_cmd *cmd, const char *str, size_t *indices,
						char *result);

int					check_quote(char arg, int in_quotes);
void				add_final_arg(t_cmd *cmd, const char *arg, size_t start);
int					contains_quotes(const char *arg);
int					redirection_contains_quotes(const char *arg);
int					add_redirection(t_cmd *cmd, int i, const char *arg,
						char *redir);
void				convert_redirection(const char *arg, t_cmd *cmd);
void				free_token_arg(t_cmd *cmd, int i);
void				stock_token(t_cmd *cmd);
int					is_quote(char c);

int					check_syntax_one(t_token **tokens, int len_tokens, int i);
int					is_redirection(t_token **tokens);
int					other_error_one(int i, int len_tokens, t_token **tokens);
int					other_error_two_out(int i, t_token **tokens);
int					other_error_two(int i, t_token **tokens);
int					other_error_third(int i, t_token **tokens);

int					set_index_redir(int i, t_token **tokens, int len_tokens);
int					set_index_redir_other(int i, t_token **tokens);
int					set_index_syntax(t_token **tokens, int len_tokens);

int					check_signal_exit_hd(char *input, int *fd,
						int is_last_stdin, char *delimiter);
int					checks_if_stdin(int i, t_token **token, int len);
char				*write_fd_input_rd(t_token **token, char *input,
						int is_last_stdin, int *fd);
void				count_len_value(char *var_name, t_env **env, int *len);
int					handle_env_variable(int *i, int *len, char *input,
						t_env **env);
int					handle_digit_variable(int *i, char *input);
int					handle_double_dollar(int *i, int *len, char *input);
char				*add_input_final(char *input, char *result);
int					handle_double_dollar_add(int *index, char *input,
						char *result);
char				*initialize_expand_result(t_env **env, char *input,
						int *len, int *index);
int					count_var(char *input, int i);
void				process_env_variable(char *input, int *index, t_env **env,
						char *result);
int					handle_redirections(t_token **tokens, int *k,
						int len_tokens);
void				check_set_status_error(char **tmp_cmd, int *flags,
						char *command, t_env **env);
void				handle_stat_not_path(char **tmp_cmd, char *command,
						t_env **env);
void				execute_if_executable(t_env **env, char **tmp_cmd,
						char *join_path, char *command);
void				execute_cmd_clear(t_env **env);
void				execute_clear_term(t_env **env, char **tmp_cmd);
#endif
