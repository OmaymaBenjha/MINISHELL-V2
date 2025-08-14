NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror #-g3 -fsanitize=address
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
	   trigger.c \
       EXECUTION/builtins_dispatch.c \
       EXECUTION/env_utils.c \
	    EXECUTION/env_utils_helper.c \
       EXECUTION/executor.c \
       EXECUTION/path_finder.c \
       EXECUTION/redirections.c \
	   EXECUTION/handler.c \
	   EXECUTION/executor_helper.c \
       EXECUTION/builtins/ft_cd.c \
       EXECUTION/builtins/ft_echo.c \
       EXECUTION/builtins/ft_env.c \
       EXECUTION/builtins/ft_exit.c \
       EXECUTION/builtins/ft_export.c \
       EXECUTION/builtins/ft_pwd.c \
       EXECUTION/builtins/ft_unset.c \
       EXECUTION/builtins/helpers.c \
       EXPAND/expand.c \
       EXPAND/g_expand.c \
       EXPAND/helper.c \
       EXPAND/quotes_removal.c \
       EXPAND/utils.c \
       EXPAND/utils_locking.c \
       EXPAND/utils_1.c \
       EXPAND/utils2.c \
       GARBAGE_COLLECTOR/gc_mall.c \
       GARBAGE_COLLECTOR/gc_mall2.c \
       HEREDOC/processor.c \
       HEREDOC/helpers.c \
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
       tools/strings.c \
       tools/strings1.c \
       tools/strings2.c \
       tools/strings3.c

HEADERS = parsing.h \
          EXECUTION/execution.h \
          EXECUTION/builtins/builtins.h

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) $(CFLAGS) -o $(NAME)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re