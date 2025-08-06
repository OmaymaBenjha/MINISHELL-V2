/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oben-jha <oben-jha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 10:59:44 by oben-jha          #+#    #+#             */
/*   Updated: 2025/08/06 20:29:09 by oben-jha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int is_void(char *s)
{
    if (!s)
        return (1);
    while (*s)
    {
        if (!ft_isspace(*s))
            return (0);
        s++;
    }
       
    return (1);
}
bool is_fully_quoted(const char *arg)
{
    size_t len;

    if (!arg)
        return (false);
    len = ft_strlen(arg);
    if (len < 2)
        return (false);
    if (arg[0] == '\"' && arg[len - 1] == '\"')
        return (true);
    if (arg[0] == '\'' && arg[len - 1] == '\'')
        return (true);
    return (false);
}
bool	has_quotes(const char *str)
{
	if (!str)
		return (false);
	while (*str)
	{
		if (*str == '\'' || *str == '\"')
			return (true);
		str++;
	}
	return (false);
}

bool is_valid_ass(const char *arg, int *is_q)
{
    char *equal_sign;
    int i;
    int is_quoted_value;

    if (!arg)
        return (false);
    is_quoted_value = 0;
    equal_sign = ft_strchr(arg, '=');
    if (equal_sign == NULL || equal_sign == arg)
        return (false);
    if (equal_sign[1] == '\"' ||  equal_sign[1] == '\'')
         (is_quoted_value = 1, (*is_q) = 1);     
    if (!ft_isalpha(arg[0]) && arg[0] != '_')
        return (false);
    i = 1;
    while (&arg[i] < equal_sign)
    {
        if (!ft_isalnum(arg[i]) && arg[i] != '_' && arg[i] != '+'
                && is_quoted_value == 0)
            return (false);
        i++;
    }
    return (true);
}
int it_has_tab(char *s)
{
    int i;
    
    i = 0;
    while(s[i])
    {
        if (s[i] == '\t')
            return (1);
        i++;
    }
    return (0);
}