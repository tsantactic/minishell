NAME = minishell
CC = gcc -g -pedantic
CFLAGS = -Wall -Wextra -Werror
SRCS =	srcs/minishell.c srcs/parsing.c srcs/utils_functions.c \
		srcs/parsing_utils.c srcs/stock_args.c srcs/lexing.c srcs/type_args.c \
		srcs/remove_and_expand.c srcs/execution_command.c srcs/execution_utils.c \
		srcs/pipe_normal.c srcs/syntax.c srcs/redir_exec.c
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
