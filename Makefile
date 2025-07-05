NAME = minishell
CC = gcc
CFLAGS = -Wall -Wextra -Werror  -fsanitize=address -g
RM = rm -f

SRCS =	main.c \
		TOKENIZER/tokenizer.c \
		TOKENIZER/tools.c \
		TOKENIZER/nodes.c \
		TOKENIZER/words.c \
		TOKENIZER/operators.c \
		TOOLS/strings.c \
		PARSER/nodes/cmd_nodes.c \
		PARSER/error_handler.c \
		PARSER/processor.c \
		PARSER/redirections.c \
		PARSER/nodes/redir_nodes.c \
		GARBAGE_COLLECTOR/gc_mall.c \
		HEREDOC/processor.c \
		EXPAND/expand.c \
		EXPAND/quotes_removal.c \
		EXPAND/utils.c \
		EXPAND/g_expand.c \
		EXECUTION/executor.c \
		EXECUTION/builtins.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) -lreadline

%.o: %.c
	$(CC) $(CFLAGS) -I. -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
