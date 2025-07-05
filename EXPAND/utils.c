#include "parsing.h" 

static size_t	count_stripped_len(const char *str)
{
	size_t	i;
	size_t	len;
	bool	in_squote;
	bool	in_dquote;

	i = 0;
	len = 0;
	in_squote = false;
	in_dquote = false;
	while (str[i])
	{
		if (str[i] == '\'' && !in_dquote)
			in_squote = !in_squote;
		else if (str[i] == '\"' && !in_squote)
			in_dquote = !in_dquote;
		else
			len++;
		i++;
	}
	return (len);
}

char    *strip_quotes(const char *str)
{
	size_t	i;
	size_t	j;
	char	*cleaned;
	bool	in_squote;
	bool	in_dquote;

	if (!str)
		return (NULL);
	cleaned = gc_mall(count_stripped_len(str) + 1);
	if (!cleaned)
		return (NULL);
	i = 0;
	j = 0;
	in_squote = false;
	in_dquote = false;
	while (str[i])
	{
		if (str[i] == '\'' && !in_dquote)
			in_squote = !in_squote;
		else if (str[i] == '\"' && !in_squote)
			in_dquote = !in_dquote;
		else
			cleaned[j++] = str[i];
		i++;
	}
	cleaned[j] = '\0';
	return (cleaned);
}

static int	count_digits(int n)
{
	int	count = 1;

	while (n / 10)
	{
		n /= 10;
		count++;
	}
	return (count);
}

char	*ft_itoa(int n)
{
	char		*str;
	long		num;
	int			len;
	int			is_negative;

	num = n;
	is_negative = (num < 0);
	if (is_negative)
		num = -num;
	len = count_digits(num) + is_negative;
	str = malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	str[len] = '\0';
	while (len-- > 0)
	{
		str[len] = '0' + (num % 10);
		num /= 10;
		if (len == 0 && is_negative)
			str[0] = '-';
	}
	return (str);
}
