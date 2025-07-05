#include "parsing.h"

static char	*get_quoted_word(char *line, int *i)
{
	char	quote_char;
	int		start;
	char	*word;

	quote_char = line[*i];
	start = *i;
	(*i)++;
	while (line[*i] && line[*i] != quote_char)
		(*i)++;
	if (line[*i] == '\0')
	{
		ft_putstr_fd("minishell: syntax error: unclosed quote\n", 2);
		return (NULL);
	}
	(*i)++;
	word = ft_substr(line, start, *i - start);
	gc_add_pt(word);
	return (word);
}

static char	*get_regular_word(char *line, int *i)
{
	int		start;
	char	*word;

	start = *i;
	while (line[*i] && !ft_isspace(line[*i]) && !is_metachar(line[*i]) \
		&& line[*i] != '\'' && line[*i] != '\"')
	{
		(*i)++;
	}
	word = ft_substr(line, start, *i - start);
	gc_add_pt(word);
	return (word);
}

t_token	*get_word_token(char *line, int *i)
{
	char	*word_value;

	if (line[*i] == '\'' || line[*i] == '\"')
	{
		word_value = get_quoted_word(line, i);
		if (word_value == NULL)
			return (NULL);
	}
	else
	{
		word_value = get_regular_word(line, i);
	}
	return (create_token(TOKEN_WORD, word_value));
}