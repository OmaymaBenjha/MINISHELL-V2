#include "parsing.h"
#include <stdlib.h>

static void	append_char(char **s, char c)
{
	char	to_append[2];

	to_append[0] = c;
	to_append[1] = '\0';
	*s = gc_strjoin(*s, to_append);
}

static char	*extract_variable_name(const char *str)
{
	int	i;

	i = 0;
	if (ft_isalpha(str[i]) || str[i] == '_')
	{
		i++;
		while (ft_isalnum(str[i]) || str[i] == '_')
			i++;
	}
	return (gc_substr(str, 0, i));
}

static void	handle_dollar_expansion(char **new_str, const char *str, int *i,
		t_shell *shell)
{
	char	*key;
	char	*value;

	(*i)++;
	if (str[*i] == '?')
	{
		*new_str = gc_strjoin(*new_str, gc_itoa(shell->last_exit_status));
		(*i)++;
		return ;
	}
	key = extract_variable_name(&str[*i]);
	if (!key || *key == '\0')
	{
		append_char(new_str, '$');
		return ;
	}
	value = my_getenv(key, shell->envp);
	if (value)
	{
		*new_str = gc_strjoin(*new_str, value);
	}
	*i += ft_strlen(key);
}

char	*expander(char *str, t_shell *shell)
{
	char	*new_str;
	int		i;
	bool	in_dquote;
	bool	in_squote;

	if (!str)
		return (NULL);
	new_str = gc_strdup("");
	i = 0;
	in_dquote = false;
	in_squote = false;
	while (str[i])
	{
		if (str[i] == '\'' && !in_dquote)
			in_squote = !in_squote;
		else if (str[i] == '\"' && !in_squote)
			in_dquote = !in_dquote;
		if (str[i] == '$' && !in_squote)
			handle_dollar_expansion(&new_str, str, &i, shell);
		else
		{
			append_char(&new_str, str[i]);
			i++;
		}
	}
	return (new_str);
}
