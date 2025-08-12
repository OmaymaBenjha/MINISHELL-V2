/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strings3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oben-jha <oben-jha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 12:03:15 by oben-jha          #+#    #+#             */
/*   Updated: 2025/08/11 12:05:05 by oben-jha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	populate_split(char **arr, char const *s, char c)
{
	size_t	i;
	size_t	j;
	size_t	start;

	i = 0;
	j = 0;
	while (s[i])
	{
		if (s[i] != c)
		{
			start = i;
			while (s[i] && s[i] != c)
				i++;
			arr[j++] = gc_substr(s, start, i - start);
		}
		else
			i++;
	}
	arr[j] = NULL;
}
