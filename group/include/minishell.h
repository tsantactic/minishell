/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sandriam <sandriam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 16:41:01 by sandriam          #+#    #+#             */
/*   Updated: 2024/11/28 11:22:39 by sandriam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../Libft/libft.h"
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
# include "tokens.h"
# include "executions.h"

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
} t_token_type;

typedef struct s_token
{
    char *value;
    t_token_type type;
	t_token_type in_quote;
} t_token;

typedef struct s_cmd
{
	t_token **tokens;
	t_token ***commands_arg;
	t_token_type type;
	t_token_type type_token;
	char	*cmd;
	char	**token_arg;
	char	**args;
	char	**lst_env;
	int 	len_tokens;
	int		len_arg;

	int **pipe_heredoc;
	int count_heredoc;
	int stdout_backup;
	int stdin_backup;

	int		in_single_quote;
	int		in_double_quote;
	int		in_word;
	int		redirect_count;
	int		var_start;
	int		start;
	int		arg_index;
	size_t nb_pipe;
	int state;
	int	is_cmd;
	int infile;
	int outfile;
	int outapp;
	int heredoc;
	int index;
	int is_empty;
	int index_command;

}			t_cmd;

void		ft_free(char **str);
char		*ft_strndup(const char *s, int n);
char		*ft_strjoin_copy(char const *s1, char const *s2);
char	*count_len_cleaned(t_cmd *cmd, const char *str, char **env,t_token_type type);
void 		loop_readline(t_cmd *cmd, char **env);
int			lexing_arg(char *line, t_cmd *cmd, char **env);
void		stock_token(t_cmd *cmd);
int			count_arg(char *s, t_cmd *cmd);
void		stock_arg(char *s, t_cmd *cmd);
char 		*remove_quotes_and_expand(t_cmd *cmd, const char *str, char **env, t_token_type type);

int			check_error_arg(t_token **tokens, int len_tokens);
int 		set_index_syntax(t_token **tokens, int len_tokens);

void	sig_handler(int signum);
void	sig_quit_handler(int signum);
int set_st(int state);
#endif
