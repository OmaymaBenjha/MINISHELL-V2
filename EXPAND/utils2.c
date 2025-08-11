/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oben-jha <oben-jha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 08:30:32 by oben-jha          #+#    #+#             */
/*   Updated: 2025/08/11 11:01:29 by oben-jha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static void	split_with_tab(char *sp, t_arg_list **head)
{
	int		i;
	char	**extra_split;

	i = 0;
	extra_split = ft_split(sp, '\t');
	while (extra_split && extra_split[i] && (extra_split[i][0] != '$'
		&& extra_split[i][1] != '\"' && extra_split[i][1] != '\'')
		&& (extra_split[i][1] != '\'' && (extra_split[i][1] != '\"')))
	{
		add_arg_to_list(head, extra_split[i]);
		i++;
	}
}

void	resplit_replace(char *s, t_arg_list **head)
{
	int	i;

	i = 0;
	if (s[0] != '\0' && ft_strcmp(s, "$\"\"") != 0
		&& ft_strcmp(s, "$\'\'") != 0
		&& ft_strcmp(s, "\"\"") != 0
		&& ft_strcmp(s, "\'\'") != 0)
	{
		while (s[i] != '\0')
		{
			if (s[i] == '$' && (s[i + 1] == '\'' || s[i + 1] == '\"'))
				s[i] = '\"';
			i++;
		}
		if (it_has_tab(s))
			split_with_tab(s, head);
		else
			add_arg_to_list(head, s);
	}
}
