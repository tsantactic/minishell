NAME = program
CC = gcc
CFLAGS = -Wall -Wextra -Werror
SRCS =	test_function.c
OBJS = $(SRCS:.c=.o)
all: $(NAME)
INCLUDES = -I include
$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -L/usr/local/lib -I/usr/local/include -lreadline -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -L/usr/local/lib -I/usr/local/include -lreadline -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
