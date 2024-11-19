/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sandriam <sandriam@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 16:41:01 by sandriam          #+#    #+#             */
/*   Updated: 2024/11/19 10:20:49 by sandriam         ###   ########.fr       */
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
	char	*cmd;
	char	**token_arg;
	char	**args;
	int 	len_tokens;
	int		len_arg;
	char	*input_file;
	int		output_file;
	char	*env_value;
	int		append_output;
	int		has_pipe;
	char	*pipe_cmd;
	int		arg_count;

	int		in_single_quote;
	int		in_double_quote;
	int		in_word;
	int		redirect_count;
	int		var_start;
	int		start;
	int		arg_index;
	size_t nb_pipe;
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

int			parse_cmd(char *line, t_cmd *cmd, char **env);
void		stock_token(t_cmd *cmd);
int			count_arg(char *s, t_cmd *cmd);
void		stock_arg(char *s, t_cmd *cmd);
char 		*remove_quotes_and_expand(t_cmd *cmd, const char *str, char **env, t_token_type type);
void free_tokens_and_args(t_cmd *cmd);

int			check_error_arg(t_token **tokens, int len_tokens);
#endif
