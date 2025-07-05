#include "parsing.h"

int	syntax_error_handler(char *token_value)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	if (token_value)
		ft_putstr_fd(token_value, 2);
	else
		ft_putstr_fd("newline", 2);
	ft_putstr_fd("'\n", 2);
	return (0);
}