/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oben-jha <oben-jha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 19:15:46 by oben-jha          #+#    #+#             */
/*   Updated: 2025/07/06 19:16:10 by oben-jha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

t_token	*tokenizer(char *line)
{
	t_token	*tokens;
	t_token	*new_token;
	int		i;

	tokens = NULL;
	i = 0;
	while (line[i])
	{
		if (ft_isspace(line[i]))
		{
			i++;
			continue ;
		}
		if (line[i] == '$' && (line[i + 1] == '\"' || line[i + 1] == '\''))
			i++;
		if (is_metachar(line[i]))
			new_token = get_operator_token(line, &i);
		else
			new_token = get_word_token(line, &i);
		if (new_token == NULL)
			return (NULL);
		if (new_token == (void *)1)
			continue ;
		add_token_back(&tokens, new_token);
	}
	add_token_back(&tokens, create_token(TOKEN_EOF, NULL));
	return (tokens);
}
