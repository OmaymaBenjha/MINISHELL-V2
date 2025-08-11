/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aziane <aziane@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 23:08:26 by oben-jha          #+#    #+#             */
/*   Updated: 2025/08/11 22:33:57 by aziane           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

# define SQUOTE_LOCK '\x03'
# define DQUOTE_LOCK '\x04'



typedef struct s_shell
{
	char	**envp;
	int		last_exit_status;
}	t_shell;

typedef struct s_gc_node
{
	void				*pt;
	struct s_gc_node	*next;
}	t_gc_node;

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
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
}	t_token;

typedef enum e_redir_type
{
	REDIR_NONE,
	REDIR_INPUT,
	REDIR_OUTPUT_TRUNC,
	REDIR_OUTPUT_APPEND,
	REDIR_HEREDOC
}	t_redir_type;

typedef struct s_redir
{
	t_redir_type	type;
	char			*del_or_fname;
	int				heredoc_fd;
	bool			expand_in_heredoc;
	struct s_redir	*next;
}	t_redir;

typedef struct s_command
{
	char				**args;
	t_redir				*redirections;
	struct s_command	*next_piped_command;
}	t_command;

typedef struct s_arg_list
{
	char				*arg;
	struct s_arg_list	*next;
}	t_arg_list;

typedef struct s_exp_data
{
	t_shell				*shell;
	int					*flag;
	struct s_exp_data	*next;
	bool				exp_f;
}	t_exp_data;

void		gc_add_pt(void *pt);
void		*gc_mall(size_t size);
char		*gc_strdup(char *str);
void		gc_freed(void);
char		*gc_substr(char const *s, unsigned int start, size_t len);
char		*gc_strjoin(char const *s1, char const *s2);
char		*gc_itoa(int n);
t_token		*tokenizer(char *line, int *status);
t_token		*create_token(t_token_type type, char *value);
void		add_token_back(t_token **list, t_token *new_token);
t_token		*get_operator_token(char *line, int *i);
t_token		*get_word_token(char *line, int *i, int *status);
t_command	*parser(t_token *tokens, int *status);
int			syntax_error_handler(char *token_value, int *status);
t_command	*create_command_node(void);
void		add_command_node_back(t_command **list, t_command *new_cmd);
t_redir		*create_redir_node(t_token_type type, char *filename);
void		add_redir_node_back(t_redir **list, t_redir *new_redir);
int			handle_redirection(t_command *cmd, t_token **current_token, int *s);
void		heredoc_error(void);
int			process_heredoc_pipe(t_command *cmds_head, t_shell *shell);
char		*final_delim(char *delim);
void		heredoc_sigint_handler(int sig);
char		*maybe_expand_line(char *line, bool flag, t_shell *shell);
char		*expander(char *str, t_shell *shell, int *exp);
void		quote_remover(t_command *cmd_list);
char		*strip_quotes(const char *str);
int			main_expand(t_command *cmds, t_shell *shell);
char		*my_getenv(const char *name, char **env);
void		add_arg_to_list(t_arg_list **head, char *arg_val);
char		**convert_list_to_array(t_arg_list *head);
void		init_data(t_exp_data **data_ptr, t_shell *shell, int *exp);
int			flag_redir_error(char *exp, char *org, int count);
char		*lock_quotes(const char *value);
void		unlock_quotes(char *arg);
bool		is_fully_quoted(const char *arg);
bool		is_valid_ass(const char *arg, int *is_quo);
int			it_has_tab(char *s);
bool		has_quotes(const char *str);
void		resplit_replace(char *splited, t_arg_list **head);
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
void		executor(t_command *commands, t_shell *shell);
char		**dupenv(char **envp);
void		free_env(char **envp);
int			ft_atoi(const char *str);
int			is_void(char *s);
void		populate_split(char **arr, char const *s, char c);
int			ft_isspace(char c);
int			is_metachar(char c);
char		**ft_split(char const *s, char c);
char		*ft_strchr(const char *s, int c);
void		initialize_shell(t_shell *shell, char **envp);
void		process_input_line(char *line, t_shell *shell);

#endif