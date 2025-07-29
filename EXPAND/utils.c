/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oben-jha <oben-jha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 23:15:34 by oben-jha          #+#    #+#             */
/*   Updated: 2025/07/13 23:15:34 by oben-jha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

static void	strip_loop(const char *str, char *cleaned)
{
	size_t	i;
	size_t	j;
	bool	in_squote;
	bool	in_dquote;

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
}

char	*strip_quotes(const char *str)
{
	char	*cleaned;

	if (!str)
		return (NULL);
	cleaned = gc_mall(count_stripped_len(str) + 1);
	if (!cleaned)
		return (NULL);
	strip_loop(str, cleaned);
	return (cleaned);
}
