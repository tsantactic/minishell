NAME = minishell

CC = gcc -g -pedantic

CFLAGS = -Wall -Wextra -Werror

LIBFT_DIR = ./libft

LIBFT = $(LIBFT_DIR)/libft.a

INCLUDES = -I include -I $(LIBFT_DIR)/include

SRCS =	srcs/main.c \
		srcs/signal.c srcs/free_all.c \
		srcs/lexing/tokenisation.c srcs/lexing/lexers.c srcs/lexing/stock_args.c srcs/lexing/lexing_utils.c srcs/lexing/type_args.c srcs/lexing/remove_and_expand.c \
		srcs/executions/executions.c srcs/executions/execution_with_pipe.c srcs/executions/execution_utils.c srcs/executions/executions_utils_supp.c\
		srcs/executions/execution_simple_cmd.c srcs/executions/parsing_command_simple.c srcs/executions/parsing_arg_builtins.c \
		srcs/executions/execute_cmd_simple.c srcs/executions/all_exec_state.c\
		srcs/syntax.c srcs/utils_functions.c \
		srcs/env_list.c \
		srcs/redirections/heredoc.c \
		srcs/redirections/redirections.c \
		srcs/redirections/redirections_with_pipe.c \
		srcs/builtins/builtins_cd.c srcs/builtins/builtins_echo.c srcs/builtins/builtins_pwd.c \
		srcs/builtins/builtins_exit.c srcs/builtins/builtins_export.c srcs/builtins/builtins_unset.c \
		srcs/builtins/builtins_utils.c srcs/builtins/builtins_env.c \

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
