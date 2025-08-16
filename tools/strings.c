/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strings.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oben-jha <oben-jha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 11:36:53 by oben-jha          #+#    #+#             */
/*   Updated: 2025/08/11 11:36:53 by oben-jha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	ft_isalpha(int c)
{
	return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

int	ft_isalnum(int c)
{
	return (ft_isalpha(c) || (c >= '0' && c <= '9'));
}

char	*ft_strdup(char *value)
{
	char	*result;
	int		i;
	int		len;

	if (!value)
		return (NULL);
	len = ft_strlen(value);
	result = malloc(sizeof(char) * (len + 1));
	if (!result)
		return (NULL);
	i = 0;
	while (value[i] != '\0')
	{
		result[i] = value[i];
		i++;
	}
	result[i] = '\0';
	return (result);
}

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	if (!s)
		return (0);
	while (s[i])
		i++;
	return (i);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	char	*r;
	size_t	s_len;

	if (!s)
		return (NULL);
	s_len = ft_strlen(s);
	if (s_len < start)
		return (gc_strdup(""));
	if (len > s_len - start)
		len = s_len - start;
	r = malloc(len + 1);
	if (!r)
		return (NULL);
	i = 0;
	while (i < len)
	{
		r[i] = s[start + i];
		i++;
	}
	r[i] = '\0';
	return (r);
}
