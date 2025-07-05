#include "parsing.h"

int	ft_isspace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || \
			c == '\v' || c == '\f' || c == '\r');
}

int	is_metachar(char c)
{
	return (c == '|' || c == '<' || c == '>');
}
int	is_unsupported_metachar(char c)
{
	if (c == ';' || c == '\\' || c == '&' || c == '(' || c == ')' || \
		c == '{' || c == '}')
		return (1);
	return (0);
}