NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror
RM = rm -f

READLINE_PATH = /mnt/homes/aziane/homebrew/opt/readline

INCLUDES = -I. \
           -Iexecution \
           -Iexecution/builtins \
           -Iexpand \
           -Iparser \
           -Iparser/nodes \
           -Itokenizer \
           -Igarbage_collector \
           -Iheredoc \
           -Itools \
           -I$(READLINE_PATH)/include

LDFLAGS = -L$(READLINE_PATH)/lib -lreadline

SRCS = main.c \
	   trigger.c \
       execution/builtins_dispatch.c \
       execution/env_utils.c \
	   execution/env_utils_helper.c \
       execution/executor.c \
       execution/path_finder.c \
       execution/redirections.c \
	   execution/handler.c \
	   execution/executor_helper.c \
       execution/builtins/ft_cd.c \
       execution/builtins/ft_echo.c \
       execution/builtins/ft_env.c \
       execution/builtins/ft_exit.c \
       execution/builtins/ft_export.c \
       execution/builtins/ft_pwd.c \
       execution/builtins/ft_unset.c \
       execution/builtins/helpers.c \
       expand/expand.c \
       expand/g_expand.c \
       expand/helper.c \
       expand/quotes_removal.c \
       expand/utils.c \
       expand/utils_locking.c \
       expand/utils_1.c \
       expand/utils2.c \
       garbage_collector/gc_mall.c \
       garbage_collector/gc_mall2.c \
       heredoc/processor.c \
       heredoc/helpers.c \
       parser/error_handler.c \
       parser/processor.c \
       parser/redirections.c \
       parser/nodes/cmd_nodes.c \
       parser/nodes/redir_nodes.c \
       tokenizer/nodes.c \
       tokenizer/operators.c \
       tokenizer/tokenizer.c \
       tokenizer/tools.c \
       tokenizer/words.c \
       tools/strings.c \
       tools/strings1.c \
       tools/strings2.c \
       tools/strings3.c

HEADERS = parsing.h \
          execution/execution.h \
          execution/builtins/builtins.h

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