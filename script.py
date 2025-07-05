import os

# --- Corrected File Contents ---

# Corrected: Fixed major memory leaks in handle_dollar_expansion
EXPAND_EXPAND_C = """
#include "parsing.h"
#include <stdlib.h>

static void\tappend_char(char **s, char c)
{
\tchar\t*temp;
\tchar\tto_append[2];

\tto_append[0] = c;
\tto_append[1] = '\\0';
\ttemp = *s;
\t*s = ft_strjoin(temp, to_append);
\tfree(temp);
}

static char\t*extract_variable_name(const char *str)
{
\tint\ti;

\ti = 0;
\tif (ft_isalpha(str[i]) || str[i] == '_')
\t{
\t\ti++;
\t\twhile (ft_isalnum(str[i]) || str[i] == '_')
\t\t\ti++;
\t}
\treturn (ft_substr(str, 0, i));
}

static void\thandle_dollar_expansion(char **new_str, const char *str, int *i,
\t\tchar **env)
{
\tchar\t*key;
\tchar\t*value;
\tchar\t*temp_str;
\tchar\t*temp_join;

\t(void)env;
\t(*i)++;
\tif (str[*i] == '?')
\t{
\t\ttemp_str = ft_itoa(0);
\t\ttemp_join = *new_str;
\t\t*new_str = ft_strjoin(temp_join, temp_str);
\t\tfree(temp_join);
\t\tfree(temp_str);
\t\t(*i)++;
\t\treturn ;
\t}
\tkey = extract_variable_name(&str[*i]);
\tif (!key || *key == '\\0')
\t{
\t\tappend_char(new_str, '$');
\t\tfree(key);
\t\treturn ;
\t}
\tvalue = getenv(key);
\tif (value)
\t{
\t\ttemp_join = *new_str;
\t\t*new_str = ft_strjoin(temp_join, value);
\t\tfree(temp_join);
\t}
\t*i += ft_strlen(key);
\tfree(key);
}

char\t*expander(char *str, char **env)
{
\tchar\t*new_str;
\tint\t\ti;

\tif (!str)
\t\treturn (NULL);
\tnew_str = ft_strdup("");
\ti = 0;
\twhile (str[i])
\t{
\t\tif (str[i] == '$')
\t\t\thandle_dollar_expansion(&new_str, str, &i, env);
\t\telse
\t\t{
\t\t\tappend_char(&new_str, str[i]);
\t\t\ti++;
\t\t}
\t}
\tgc_add_pt(new_str);
\treturn (new_str);
}
"""

# Corrected: Replaced buggy strncmp with robust strcmp for delimiter matching
HEREDOC_PROCESSOR_C = """
#include "parsing.h"

static char\t*maybe_expand_line(char *line, bool flag, char **env)
{
\tchar\t*new_str;
\tchar\t*original_expander_result;

\tif (flag == true)
\t{
\t\toriginal_expander_result = expander(line, env);
\t\tnew_str = ft_strdup(original_expander_result);
\t}
\telse
\t\tnew_str = ft_strdup(line);
\treturn (new_str);
}

int\tprocess_heredoc_pipe(t_command *cmds_head, char **env)
{
\tt_command\t*cmd;
\tt_redir\t\t*redir;
\tint\t\t\tfd[2];
\tchar\t\t*line;
\tchar\t\t*processed_line;
\tchar\t\t*delimiter;

\tcmd = cmds_head;
\twhile (cmd)
\t{
\t\tredir = cmd->redirections;
\t\twhile (redir)
\t\t{
\t\t\tif (redir->type == REDIR_HEREDOC)
\t\t\t{
\t\t\t\tif (pipe(fd) == -1)
\t\t\t\t\treturn (perror("pipe"), 0);
\t\t\t\tdelimiter = strip_quotes(redir->delimiter_or_filename);
\t\t\t\twhile (1)
\t\t\t\t{
\t\t\t\t\tline = readline("heredoc> ");
\t\t\t\t\tif (!line || ft_strcmp(line, delimiter) == 0)
\t\t\t\t\t{
\t\t\t\t\t\tif (line)
\t\t\t\t\t\t\tfree(line);
\t\t\t\t\t\tbreak ;
\t\t\t\t\t}
\t\t\t\t\tprocessed_line = maybe_expand_line(line,
\t\t\t\t\t\t\tredir->expand_in_heredoc, env);
\t\t\t\t\twrite(fd[1], processed_line, ft_strlen(processed_line));
\t\t\t\t\twrite(fd[1], "\\n", 1);
\t\t\t\t\tfree(line);
\t\t\t\t\tfree(processed_line);
\t\t\t\t}
\t\t\t\tclose(fd[1]);
\t\t\t\tredir->heredoc_fd = fd[0];
\t\t\t}
\t\t\tredir = redir->next;
\t\t}
\t\tcmd = cmd->next_piped_command;
\t}
\treturn (1);
}
"""

# Corrected: Fixed logic to not strip quotes from heredoc delimiters
EXPAND_QUOTES_REMOVAL_C = """
#include "parsing.h"

static void\tclean_quotes_in_args(t_command *cmd)
{
\tint\t\ti;
\tchar\t*stripped;

\ti = 0;
\tif (!cmd || !cmd->args)
\t\treturn ;
\twhile (cmd->args[i])
\t{
\t\tstripped = strip_quotes(cmd->args[i]);
\t\tif (stripped)
\t\t\tcmd->args[i] = stripped;
\t\ti++;
\t}
}

static void\tclean_quotes_in_redirs(t_command *cmd)
{
\tt_redir\t*redir;
\tchar\t*stripped;

\tif (!cmd)
\t\treturn ;
\tredir = cmd->redirections;
\twhile (redir)
\t{
\t\tif (redir->type != REDIR_HEREDOC)
\t\t{
\t\t\tstripped = strip_quotes(redir->delimiter_or_filename);
\t\t\tif (stripped)
\t\t\t\tredir->delimiter_or_filename = stripped;
\t\t}
\t\tredir = redir->next;
\t}
}

void\tquote_remover(t_command *cmd_list)
{
\twhile (cmd_list)
\t{
\t\tclean_quotes_in_args(cmd_list);
\t\tclean_quotes_in_redirs(cmd_list);
\t\tcmd_list = cmd_list->next_piped_command;
\t}
}
"""

# Corrected: Added ft_strcmp and fixed ft_strncmp
TOOLS_STRINGS_C = """
#include "parsing.h"

int\tft_isalpha(int c)
{
\treturn ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

int\tft_isalnum(int c)
{
\treturn (ft_isalpha(c) || (c >= '0' && c <= '9'));
}

char\t*ft_strjoin(char const *s1, char const *s2)
{
\tchar\t*new_str;
\tsize_t\ti;
\tsize_t\tj;

\tif (!s1 || !s2)
\t\treturn (NULL);
\tnew_str = (char *)malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
\tif (!new_str)
\t\treturn (NULL);
\ti = 0;
\twhile (s1[i])
\t{
\t\tnew_str[i] = s1[i];
\t\ti++;
\t}
\tj = 0;
\twhile (s2[j])
\t{
\t\tnew_str[i + j] = s2[j];
\t\tj++;
\t}
\tnew_str[i + j] = '\\0';
\treturn (new_str);
}

int\tft_strcmp(const char *s1, const char *s2)
{
\tsize_t\ti;

\ti = 0;
\twhile (s1[i] && s2[i] && s1[i] == s2[i])
\t\ti++;
\treturn ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

int\tft_strncmp(const char *s1, const char *s2, size_t n)
{
\tsize_t\ti;

\ti = 0;
\tif (n == 0)
\t\treturn (0);
\twhile (i < n - 1 && s1[i] && s2[i] && s1[i] == s2[i])
\t\ti++;
\treturn ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

char\t*ft_strdup(char *value)
{
\tchar\t*result;
\tint\t\ti;
\tint\t\tlen;

\tif (!value)
\t\treturn (NULL);
\tlen = ft_strlen(value);
\tresult = malloc(sizeof(char) * (len + 1));
\tif (!result)
\t\treturn (NULL);
\ti = 0;
\twhile (value[i] != '\\0')
\t{
\t\tresult[i] = value[i];
\t\ti++;
\t}
\tresult[i] = '\\0';
\treturn (result);
}

size_t\tft_strlen(const char *s)
{
\tsize_t\ti;

\ti = 0;
\tif (!s)
\t\treturn (0);
\twhile (s[i])
\t\ti++;
\treturn (i);
}

void\tft_putstr_fd(const char *s, int fd)
{
\tif (s)
\t\twrite(fd, s, ft_strlen(s));
}

char\t*ft_substr(char const *s, unsigned int start, size_t len)
{
\tsize_t\ti;
\tchar\t*r;
\tsize_t\ts_len;

\tif (!s)
\t\treturn (NULL);
\ts_len = ft_strlen(s);
\tif (s_len < start)
\t\treturn (ft_strdup(""));
\tif (len > s_len - start)
\t\tlen = s_len - start;
\tr = malloc(len + 1);
\tif (!r)
\t\treturn (NULL);
\ti = 0;
\twhile (i < len)
\t{
\t\tr[i] = s[start + i];
\t\ti++;
\t}
\tr[i] = '\\0';
\treturn (r);
}
"""

# Corrected: Added the prototype for ft_strcmp
PARSING_H = """
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
\tvoid\t\t\t\t*pt;
\tstruct s_gc_node\t*next;
}t_gc_node;

typedef enum e_token_type
{
\tTOKEN_WORD,
\tTOKEN_PIPE,
\tTOKEN_REDIR_IN,
\tTOKEN_REDIR_OUT,
\tTOKEN_REDIR_APPEND,
\tTOKEN_REDIR_HEREDOC,
\tTOKEN_NEWLINE,
\tTOKEN_EOF,
\tTOKEN_ERROR
}t_token_type;

typedef struct s_token
{
\tt_token_type\ttype;
\tchar\t\t\t*value;
\tstruct s_token\t*next;
}t_token;

typedef enum e_redir_type
{
\tREDIR_NONE,
\tREDIR_INPUT,
\tREDIR_OUTPUT_TRUNC,
\tREDIR_OUTPUT_APPEND,
\tREDIR_HEREDOC
}t_redir_type;

typedef struct s_redir
{
\tt_redir_type\ttype;
\tchar\t\t\t*delimiter_or_filename;
\tint\t\t\t\theredoc_fd;
\tbool\t\t\texpand_in_heredoc;
\tstruct s_redir\t*next;
}t_redir;

typedef struct s_command
{
\tchar\t\t\t\t**args;
\tt_redir\t\t\t\t*redirections;
\tstruct s_command\t*next_piped_command;
}t_command;

void\t\tgc_add_pt(void *pt);
void\t\t*gc_mall(size_t size);
char\t\t*gc_strdup(char *str);
void\t\tgc_freed(void);

// TOKENIZER PROTOTYPES
t_token\t\t*tokenizer(char *line);
t_token\t\t*create_token(t_token_type type, char *value);
void\t\tadd_token_back(t_token **list, t_token *new_token);
t_token\t\t*get_operator_token(char *line, int *i);
t_token\t\t*get_word_token(char *line, int *i);

// PARSER PROTOTYPES
t_command\t*parser(t_token *tokens);
int\t\t\tsyntax_error_handler(char *token_value);
t_command\t*create_command_node(void);
void\t\tadd_command_node_back(t_command **list, t_command *new_cmd);
t_redir\t\t*create_redir_node(t_token_type type, char *filename);
void\t\tadd_redir_node_back(t_redir **list, t_redir *new_redir);
int\t\t\thandle_redirection(t_command *cmd, t_token **current_token);

// HEREDOC PROTOTYPES
int\t\t\tprocess_heredoc_pipe(t_command *cmds_head, char **env);

// EXPAND PROTOTYPES
char\t\t*expander(char *str, char **env);
void\t\tquote_remover(t_command *cmd_list);
char\t\t*strip_quotes(const char *str);
void\t\tglobal_expand(t_command *cmds_head, char **env);

// TOOLS/STRINGS PROTOTYPES
char\t\t*ft_strdup(char *value);
int\t\t\tft_strncmp(const char *s1, const char *s2, size_t n);
int\t\t\tft_strcmp(const char *s1, const char *s2);
size_t\t\tft_strlen(const char *s);
void\t\tft_putstr_fd(const char *s, int fd);
char\t\t*ft_substr(char const *s, unsigned int start, size_t len);
char\t\t*ft_strjoin(char const *s1, char const *s2);
int\t\t\tft_isalpha(int c);
int\t\t\tft_isalnum(int c);
char\t\t*ft_itoa(int n);

// TOOLS/CHECKERS PROTOTYPES
int\t\t\tft_isspace(char c);
int\t\t\tis_metachar(char c);
int\t\t\tis_unsupported_metachar(char c);

#endif
"""

# --- Main Script Logic ---

def apply_fixes():
    """Finds and overwrites project files with their corrected versions."""
    
    files_to_fix = {
        "EXPAND/expand.c": EXPAND_EXPAND_C,
        "HEREDOC/processor.c": HEREDOC_PROCESSOR_C,
        "EXPAND/quotes_removal.c": EXPAND_QUOTES_REMOVAL_C,
        "TOOLS/strings.c": TOOLS_STRINGS_C,
        "parsing.h": PARSING_H
    }
    
    print("--- Applying Fixes to Minishell Project ---")
    
    for filepath, content in files_to_fix.items():
        if os.path.exists(filepath):
            try:
                with open(filepath, "w") as f:
                    f.write(content)
                print(f"[SUCCESS] Corrected '{filepath}'")
            except IOError as e:
                print(f"[ERROR] Could not write to '{filepath}': {e}")
        else:
            print(f"[WARNING] File not found, skipping: '{filepath}'")
            
    print("\n--- Fixes Applied ---")
    print("Please run 'make clean && make' to recompile your project.")

if __name__ == "__main__":
    apply_fixes()