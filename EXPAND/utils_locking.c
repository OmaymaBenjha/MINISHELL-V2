/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_locking.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oben-jha <oben-jha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 12:10:00 by oben-jha          #+#    #+#             */
/*   Updated: 2025/08/11 13:02:21 by oben-jha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static void	lock_quotes_loop(char *locked, const char *value)
{
	int		i;
	bool	in_squote;
	bool	in_dquote;

	i = 0;
	in_squote = false;
	in_dquote = false;
	while (value[i])
	{
		if (value[i] == '\'' && !in_dquote)
		{
			locked[i] = SQUOTE_LOCK;
			in_squote = !in_squote;
		}
		else if (value[i] == '\"' && !in_squote)
		{
			locked[i] = DQUOTE_LOCK;
			in_dquote = !in_dquote;
		}
		else
			locked[i] = value[i];
		i++;
	}
	locked[i] = '\0';
}

char	*lock_quotes(const char *value)
{
	char	*locked_str;

	if (!value)
		return (NULL);
	locked_str = gc_mall(ft_strlen(value) + 1);
	if (!locked_str)
		return (NULL);
	lock_quotes_loop(locked_str, value);
	return (locked_str);
}

void	unlock_quotes(char *arg)
{
	int	i;

	if (!arg)
		return ;
	i = 0;
	while (arg[i])
	{
		if (arg[i] == SQUOTE_LOCK)
			arg[i] = '\'';
		else if (arg[i] == DQUOTE_LOCK)
			arg[i] = '\"';
		i++;
	}
}
