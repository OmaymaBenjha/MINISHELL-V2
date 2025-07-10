/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   words.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oben-jha <oben-jha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 19:13:51 by oben-jha          #+#    #+#             */
/*   Updated: 2025/07/06 19:13:56 by oben-jha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static char	*extract_quoted_part(char *line, int *i)
{
	char	quote_char;
	int		start;

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
	return (gc_substr(line, start, *i - start));
}

static char	*extract_unquoted_part(char *line, int *i)
{
	int	start;

	start = *i;
	while (line[*i] && !ft_isspace(line[*i]) && !is_metachar(line[*i])
		&& line[*i] != '\'' && line[*i] != '"')
		(*i)++;
	return (gc_substr(line, start, *i - start));
}

t_token	*get_word_token(char *line, int *i)
{
	char	*word_so_far;
	char	*next_part;
	char	*temp;

	word_so_far = gc_strdup("");
	if (!word_so_far)
		return (NULL);
	while (line[*i] && !ft_isspace(line[*i]) && !is_metachar(line[*i]))
	{
		if (line[*i] == '\'' || line[*i] == '"')
			next_part = extract_quoted_part(line, i);
		else
			next_part = extract_unquoted_part(line, i);
		if (!next_part)
			return (NULL);
		temp = word_so_far;
		word_so_far = gc_strjoin(temp, next_part);
	}
	if (ft_strlen(word_so_far) == 0)
		return ((void *)1);
	return (create_token(TOKEN_WORD, word_so_far));
}
