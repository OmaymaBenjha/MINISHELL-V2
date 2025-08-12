/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_mall2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oben-jha <oben-jha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 15:45:12 by oben-jha          #+#    #+#             */
/*   Updated: 2025/07/14 20:58:00 by oben-jha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

char	*gc_itoa(int n)
{
	char	*new_str;

	new_str = ft_itoa(n);
	if (!new_str)
	{
		perror("substr failed!");
		exit(EXIT_FAILURE);
	}
	gc_add_pt(new_str);
	return (new_str);
}

void	*gc_mall(size_t size)
{
	void	*pt;

	pt = malloc(size);
	if (!pt)
	{
		perror("malloc failed!");
		exit(EXIT_FAILURE);
	}
	gc_add_pt(pt);
	return (pt);
}

char	*gc_strjoin(char const *s1, char const *s2)
{
	char	*new_str;

	if (!s1 || !s2)
		return (NULL);
	new_str = ft_strjoin(s1, s2);
	if (!new_str)
	{
		perror("substr failed!");
		exit(EXIT_FAILURE);
	}
	gc_add_pt(new_str);
	return (new_str);
}
