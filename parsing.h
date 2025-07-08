
#ifndef PARSING_H
# define PARSING_H

# include <stdlib.h>
# include <stddef.h>
# include <stdio.h>
# include <stdbool.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <sys/wait.h>

typedef struct s_gc_node
{
	void				*pt;
	struct s_gc_node	*next;
}t_gc_node;

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_APPEND,
	TOKEN_REDIR_HEREDOC,
	TOKEN_NEWLINE,
	TOKEN_EOF,
	TOKEN_ERROR
}t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
}t_token;

typedef enum e_redir_type
{
	REDIR_NONE,
	REDIR_INPUT,
	REDIR_OUTPUT_TRUNC,
	REDIR_OUTPUT_APPEND,
	REDIR_HEREDOC
}t_redir_type;

typedef struct s_redir
{
	t_redir_type	type;
	char			*delimiter_or_filename;
	int				heredoc_fd;
	bool			expand_in_heredoc;
	struct s_redir	*next;
}t_redir;

typedef struct s_command
{
	char				**args;
	t_redir				*redirections;
	struct s_command	*next_piped_command;
}t_command;

void		gc_add_pt(void *pt);
void		*gc_mall(size_t size);
char		*gc_strdup(char *str);
void		gc_freed(void);
char		*gc_substr(char const *s, unsigned int start, size_t len);
char		*gc_strjoin(char const *s1, char const *s2);
char		*gc_itoa(int n);

// TOKENIZER PROTOTYPES
t_token		*tokenizer(char *line);
t_token		*create_token(t_token_type type, char *value);
void		add_token_back(t_token **list, t_token *new_token);
t_token		*get_operator_token(char *line, int *i);
t_token		*get_word_token(char *line, int *i);

// PARSER PROTOTYPES
t_command	*parser(t_token *tokens);
int			syntax_error_handler(char *token_value);
t_command	*create_command_node(void);
void		add_command_node_back(t_command **list, t_command *new_cmd);
t_redir		*create_redir_node(t_token_type type, char *filename);
void		add_redir_node_back(t_redir **list, t_redir *new_redir);
int			handle_redirection(t_command *cmd, t_token **current_token);

// HEREDOC PROTOTYPES
int			process_heredoc_pipe(t_command *cmds_head, char **env);

// EXPAND PROTOTYPES
char		*expander(char *str, char **env);
void		quote_remover(t_command *cmd_list);
char		*strip_quotes(const char *str);
void		global_expand(t_command *cmds_head, char **env);

// TOOLS/STRINGS PROTOTYPES
char		*ft_strdup(char *value);
int			ft_strncmp(const char *s1, const char *s2, size_t n);
int			ft_strcmp(const char *s1, const char *s2);
size_t		ft_strlen(const char *s);
void		ft_putstr_fd(const char *s, int fd);
char		*ft_substr(char const *s, unsigned int start, size_t len);
char		*ft_strjoin(char const *s1, char const *s2);
int			ft_isalpha(int c);
int			ft_isalnum(int c);
char		*ft_itoa(int n);
void executor(t_command *commands, char ***envp);

// TOOLS/CHECKERS PROTOTYPES
int			ft_isspace(char c);
int			is_metachar(char c);
int			is_unsupported_metachar(char c);
char	**ft_split(char const *s, char c);
#endif

char		*ft_strchr(const char *s, int c);
