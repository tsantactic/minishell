NAME = minishell
CC = gcc -g -pedantic
CFLAGS = -Wall -Wextra -Werror
SRCS =	srcs/main.c \
		srcs/signal.c srcs/free_all.c srcs/tokenisation.c\
		srcs/lexers.c srcs/stock_args.c srcs/lexing_utils.c srcs/type_args.c \
		srcs/remove_and_expand.c srcs/executions.c srcs/execution_with_pipe.c\
		srcs/execution_simple_cmd.c srcs/parsing_command_simple.c srcs/parsing_arg_builtins.c\
		srcs/syntax.c srcs/utils_functions.c srcs/execution_utils.c
OBJS = $(SRCS:.c=.o)
all: $(LIBFT) $(NAME)
LIBFT_DIR = Libft
LIBFT = $(LIBFT_DIR)/libft.a
INCLUDES = -I include -I $(LIBFT_DIR)/include
$(LIBFT):
	cd $(LIBFT_DIR) && $(MAKE)
$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -L/usr/local/lib -I/usr/local/include -lreadline -o $(NAME) 

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -L/usr/local/lib -I/usr/local/include -lreadline -c $< -o $@

clean:
	rm -f $(OBJS)
	cd $(LIBFT_DIR) && $(MAKE) clean

fclean: clean
	rm -f $(NAME)
	cd $(LIBFT_DIR) && $(MAKE) fclean

re: fclean all

.PHONY: all clean fclean re
