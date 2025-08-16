/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operators.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oben-jha <oben-jha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 19:11:58 by oben-jha          #+#    #+#             */
/*   Updated: 2025/07/06 19:11:58 by oben-jha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

t_token	*get_operator_token(char *line, int *i)
{
	if (line[*i] == '|')
		return ((*i)++, create_token(TOKEN_PIPE, "|"));
	if (line[*i] == '>')
	{
		(*i)++;
		if (line[*i] == '>')
			return ((*i)++, create_token(TOKEN_REDIR_APPEND, ">>"));
		return (create_token(TOKEN_REDIR_OUT, ">"));
	}
	if (line[*i] == '<')
	{
		(*i)++;
		if (line[*i] == '<')
			return ((*i)++, create_token(TOKEN_REDIR_HEREDOC, "<<"));
		return (create_token(TOKEN_REDIR_IN, "<"));
	}
	return (NULL);
}
