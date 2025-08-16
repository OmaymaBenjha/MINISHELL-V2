/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oben-jha <oben-jha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 23:14:18 by oben-jha          #+#    #+#             */
/*   Updated: 2025/07/11 23:14:18 by oben-jha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	syntax_error_handler(char *token_value, int *status)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	if (token_value)
		ft_putstr_fd(token_value, 2);
	else
		ft_putstr_fd("newline", 2);
	ft_putstr_fd("'\n", 2);
	(*status) = 258;
	return (0);
}

void	heredoc_error(void)
{
	ft_putstr_fd("minishell: maximum here-document count exceeded\n", 2);
}
