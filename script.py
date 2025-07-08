import os

# --- Corrected File Contents ---

TOKENIZER_WORDS_C = """
#include "parsing.h"

static char\t*extract_quoted_part(char *line, int *i)
{
\tchar\tquote_char;
\tint\t\tstart;

\tquote_char = line[*i];
\tstart = *i;
\t(*i)++;
\twhile (line[*i] && line[*i] != quote_char)
\t\t(*i)++;
\tif (line[*i] == '\\0')
\t{
\t\tft_putstr_fd("minishell: syntax error: unclosed quote\\n", 2);
\t\treturn (NULL);
\t}
\t(*i)++;
\treturn (ft_substr(line, start, *i - start));
}

static char\t*extract_unquoted_part(char *line, int *i)
{
\tint\tstart;

\tstart = *i;
\twhile (line[*i] && !ft_isspace(line[*i]) && !is_metachar(line[*i])
\t\t&& line[*i] != '\\'' && line[*i] != '\"')
\t{
\t\t(*i)++;
\t}
\treturn (ft_substr(line, start, *i - start));
}

t_token\t*get_word_token(char *line, int *i)
{
\tchar\t*word_so_far;
\tchar\t*next_part;
\tchar\t*temp;

\tword_so_far = ft_strdup("");
\tif (!word_so_far)
\t\treturn (NULL);
\twhile (line[*i] && !ft_isspace(line[*i]) && !is_metachar(line[*i]))
\t{
\t\tif (line[*i] == '\\'' || line[*i] == '\"')
\t\t\tnext_part = extract_quoted_part(line, i);
\t\telse
\t\t\tnext_part = extract_unquoted_part(line, i);
\t\tif (!next_part)
\t\t{
\t\t\tfree(word_so_far);
\t\t\treturn (NULL);
\t\t}
\t\ttemp = word_so_far;
\t\tword_so_far = ft_strjoin(temp, next_part);
\t\tfree(temp);
\t\tfree(next_part);
\t}
\tif (ft_strlen(word_so_far) == 0)
\t{
\t\tfree(word_so_far);
\t\treturn ((void *)1);
\t}
\treturn (create_token(TOKEN_WORD, word_so_far));
}
"""

TOKENIZER_TOKENIZER_C = """
#include "parsing.h"

t_token\t*tokenizer(char *line)
{
\tt_token\t*tokens;
\tt_token\t*new_token;
\tint\t\ti;

\ttokens = NULL;
\ti = 0;
\twhile (line[i])
\t{
\t\tif (ft_isspace(line[i]))
\t\t{
\t\t\ti++;
\t\t\tcontinue ;
\t\t}
\t\tif (is_metachar(line[i]))
\t\t\tnew_token = get_operator_token(line, &i);
\t\telse
\t\t\tnew_token = get_word_token(line, &i);
\t\tif (new_token == NULL)
\t\t\treturn (NULL);
\t\tif (new_token == (void *)1)
\t\t\tcontinue;
\t\tadd_token_back(&tokens, new_token);
\t}
\tadd_token_back(&tokens, create_token(TOKEN_EOF, NULL));
\treturn (tokens);
}
"""

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
\tbool\tin_dquote;
\tbool\tin_squote;

\tif (!str)
\t\treturn (NULL);
\tnew_str = ft_strdup("");
\ti = 0;
\tin_dquote = false;
\tin_squote = false;
\twhile (str[i])
\t{
\t\tif (str[i] == '\\'' && !in_dquote)
\t\t\tin_squote = !in_squote;
\t\telse if (str[i] == '\\\"' && !in_squote)
\t\t\tin_dquote = !in_dquote;
\t\tif (str[i] == '$' && !in_squote)
\t\t\thandle_dollar_expansion(&new_str, str, &i, env);
\t\telse
\t\t{
\t\t\tappend_char(&new_str, str[i]);
\t\t\ti++;
\t\t}
\t}
\treturn (new_str);
}
"""

def apply_fixes():
    files_to_fix = {
        "TOKENIZER/words.c": TOKENIZER_WORDS_C,
        "TOKENIZER/tokenizer.c": TOKENIZER_TOKENIZER_C,
        "EXPAND/expand.c": EXPAND_EXPAND_C,
    }
    
    print("--- Applying Tokenizer and Expander Fixes ---")
    
    for filepath, content in files_to_fix.items():
        if os.path.exists(filepath):
            try:
                with open(filepath, "w") as f:
                    f.write(content)
                print("[SUCCESS] Corrected '{}'".format(filepath))
            except IOError as e:
                print("[ERROR] Could not write to '{}': {}".format(filepath, e))
        else:
            print("[WARNING] File not found, skipping: '{}'".format(filepath))
            
    print("\n--- Fixes Applied ---")
    print("Run 'make clean && make' and re-run the test script.")

if __name__ == "__main__":
    apply_fixes()