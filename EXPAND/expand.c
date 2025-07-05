
#include "parsing.h"
#include <stdlib.h>

static void	append_char(char **s, char c)
{
	char	*temp;
	char	to_append[2];

	to_append[0] = c;
	to_append[1] = '\0';
	temp = *s;
	*s = ft_strjoin(temp, to_append);
	free(temp);
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
	return (ft_substr(str, 0, i));
}

static void	handle_dollar_expansion(char **new_str, const char *str, int *i,
		char **env)
{
	char	*key;
	char	*value;
	char	*temp_str;
	char	*temp_join;

	(void)env;
	(*i)++;
	if (str[*i] == '?')
	{
		temp_str = ft_itoa(0);
		temp_join = *new_str;
		*new_str = ft_strjoin(temp_join, temp_str);
		free(temp_join);
		free(temp_str);
		(*i)++;
		return ;
	}
	key = extract_variable_name(&str[*i]);
	if (!key || *key == '\0')
	{
		append_char(new_str, '$');
		free(key);
		return ;
	}
	value = getenv(key);
	if (value)
	{
		temp_join = *new_str;
		*new_str = ft_strjoin(temp_join, value);
		free(temp_join);
	}
	*i += ft_strlen(key);
	free(key);
}

char	*expander(char *str, char **env)
{
	char	*new_str;
	int		i;

	if (!str)
		return (NULL);
	new_str = ft_strdup("");
	i = 0;
	while (str[i])
	{
		if (str[i] == '$')
			handle_dollar_expansion(&new_str, str, &i, env);
		else
		{
			append_char(&new_str, str[i]);
			i++;
		}
	}
	gc_add_pt(new_str);
	return (new_str);
}
