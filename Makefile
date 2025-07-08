NAME 		= minishell

CC 			= cc
CFLAGS 		= -Wall -Wextra -Werror
RM 			= rm -f


READLINE_PATH = /mnt/homes/oben-jha/homebrew/opt/readline

INCLUDE_DIRS = -I. \
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


LDFLAGS		= -L$(READLINE_PATH)/lib -lreadline

SRC_DIRS 	= EXECUTION \
			  EXECUTION/builtins \
			  EXPAND \
			  GARBAGE_COLLECTOR \
			  HEREDOC \
			  PARSER \
			  PARSER/nodes \
			  TOKENIZER \
			  tools

SRCS 		= $(foreach dir, $(SRC_DIRS), $(wildcard $(dir)/*.c))
SRCS	   += main.c
OBJS 		= $(SRCS:.c=.o)
HDRS		= $(wildcard $(foreach dir, $(SRC_DIRS), $(dir)/*.h)) parsing.h

.PHONY: all clean fclean re bonus

all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(OBJS) $(LDFLAGS) -o $(NAME)
	@echo "\033[32mMinishell compiled successfully!\033[0m"

%.o: %.c Makefile $(HDRS)
	@$(CC) $(CFLAGS) $(INCLUDE_DIRS) -c $< -o $@
	@echo "\033[34mCompiling: \033[0m$<"

clean:
	@$(RM) $(OBJS)
	@echo "\033[33mObject files cleaned.\033[0m"

fclean: clean
	@$(RM) $(NAME)
	@echo "\033[31mExecutable cleaned.\033[0m"

re: fclean all

bonus: all
	@echo "\033[35mBonus part not implemented in Makefile. Building main part.\033[0m"