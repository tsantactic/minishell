NAME = minishell

CC = gcc

CFLAGS = -Wall -Wextra -Werror

LIBFT_DIR = ./libft

LIBFT = $(LIBFT_DIR)/libft.a

INCLUDES = -I include -I $(LIBFT_DIR)/include

SRCS =	srcs/main.c \
		srcs/signal.c srcs/free_all.c \
		srcs/lexing/tokenisation.c srcs/lexing/lexers.c srcs/lexing/stock_args.c srcs/lexing/lexing_utils.c srcs/lexing/type_args.c srcs/lexing/remove_and_expand.c \
		srcs/lexing/move_quote.c srcs/lexing/lexing_utils2.c srcs/lexing/remove_and_expand2.c \
		srcs/lexing/stock_args_utils.c srcs/lexing/tokenisation2.c \
		srcs/lexing/type_args2.c srcs/lexing/type_args3.c \
		srcs/executions/executions.c srcs/executions/execution_with_pipe.c srcs/executions/execution_utils.c srcs/executions/executions_utils_supp.c\
		srcs/executions/execution_simple_cmd.c \
		srcs/executions/parsing_command_simple.c srcs/executions/parsing_cmd_simple_utils.c \
		srcs/executions/execute_cmd_simple.c srcs/executions/execute_cmd_simple_utils.c srcs/executions/all_exec_state.c srcs/executions/check_stdin_stdout.c\
		srcs/executions/path_not_set.c srcs/executions/path_not_bin.c srcs/executions/path_not_bin_utils.c\
		srcs/executions/executions_utils_supp2.c  \
		srcs/executions/executions_loop.c \
		srcs/executions/proccess_execution_pipe.c srcs/executions/execute_pipe_util_fourth.c\
		srcs/executions/execute_heredoc_blt.c srcs/executions/loop_heredoc_blt.c srcs/executions/parsing_arg_builtins_two.c \
		srcs/executions/parsing_arg_builtins_utils.c srcs/executions/parsing_arg_builtins.c \
		srcs/executions/execution_pipe_utils_second.c srcs/executions/execution_pipe_utils.c srcs/executions/execute_piped_commands.c\
		srcs/executions/execution_pipe_util_third.c srcs/executions/execute_pipe_util_five.c srcs/executions/execute_pipe_process_utils.c\
		srcs/executions/execute_command_pipe_utils.c srcs/executions/execution_built_pipe.c\
		srcs/syntax/syntax.c srcs/syntax/syntax_utils.c srcs/syntax/syntax_utils2.c srcs/syntax/syntax_utils3.c srcs/utils_functions.c \
		srcs/env_list.c srcs/signal_part_two.c\
		srcs/redirections/execute_heredoc_expand.c srcs/redirections/expand_heredoc.c srcs/redirections/expand_heredoc_utils.c\
		srcs/redirections/heredoc_pipe_utils.c \
		srcs/redirections/loop_heredoc_simple.c srcs/redirections/heredoc.c srcs/redirections/heredoc_simple_utils.c \
		srcs/redirections/redirections.c srcs/redirections/redirection_exec.c srcs/redirections/redirections_exec_pipe.c\
		srcs/redirections/redirections_with_pipe.c srcs/redirections/heredoc_with_pipe.c \
		srcs/builtins/builtins_cd.c srcs/builtins/builtins_echo.c srcs/builtins/builtins_pwd.c \
		srcs/builtins/builtins_exit.c srcs/builtins/builtins_export.c srcs/builtins/builtins_unset.c \
		srcs/builtins/builtins_utils.c srcs/builtins/builtins_env.c \
		srcs/builtins/free_memory.c
OBJS = $(SRCS:.c=.o)

all: $(LIBFT) $(NAME)

$(LIBFT):
	@make -C $(LIBFT_DIR)

$(NAME): $(OBJS) $(LIBFT)
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -L/usr/local/lib -I/usr/local/include -lreadline -o $(NAME) 

%.o: %.c
	@$(CC) $(CFLAGS) $(INCLUDES) -L/usr/local/lib -I/usr/local/include -lreadline -c $< -o $@

clean:
	@rm -f $(OBJS)
	@make -C $(LIBFT_DIR) clean

fclean: clean
	@rm -f $(NAME)
	@make -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
