NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror
RM = rm -f

READLINE_PATH = /mnt/homes/oben-jha/homebrew/opt/readline

INCLUDES = -I. \
           -IEXECUTION \
           -IEXECUTION/builtins \
           -IEXPAND \
           -IPARSER \
           -IPARSER/nodes \
           -ITOKENIZER \
           -IGARBAGE_COLLECTOR \
           -IHEREDOC \
           -Itools \
           -I$(READLINE_PATH)/include 


LDFLAGS = -L$(READLINE_PATH)/lib -lreadline

SRCS = main.c \
       EXECUTION/builtins_dispatch.c \
       EXECUTION/env_utils.c \
       EXECUTION/executor.c \
       EXECUTION/path_finder.c \
       EXECUTION/redirections.c \
       EXECUTION/builtins/ft_cd.c \
       EXECUTION/builtins/ft_echo.c \
       EXECUTION/builtins/ft_env.c \
       EXECUTION/builtins/ft_exit.c \
       EXECUTION/builtins/ft_export.c \
       EXECUTION/builtins/ft_pwd.c \
       EXECUTION/builtins/ft_unset.c \
       EXPAND/expand.c \
       EXPAND/g_expand.c \
       EXPAND/helper.c \
       EXPAND/quotes_removal.c \
       EXPAND/utils.c \
       GARBAGE_COLLECTOR/gc_mall.c \
       GARBAGE_COLLECTOR/gc_mall2.c \
       HEREDOC/processor.c \
       PARSER/error_handler.c \
       PARSER/processor.c \
       PARSER/redirections.c \
       PARSER/nodes/cmd_nodes.c \
       PARSER/nodes/redir_nodes.c \
       TOKENIZER/nodes.c \
       TOKENIZER/operators.c \
       TOKENIZER/tokenizer.c \
       TOKENIZER/tools.c \
       TOKENIZER/words.c \
       tools/strings.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re