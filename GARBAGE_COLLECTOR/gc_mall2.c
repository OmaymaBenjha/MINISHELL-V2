/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_mall2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oben-jha <oben-jha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 15:45:12 by oben-jha          #+#    #+#             */
/*   Updated: 2025/07/14 15:46:15 by oben-jha         ###   ########.fr       */
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

void	gc_freed(void)
{
	t_gc_node	*current;
	t_gc_node	*tmp;
	t_gc_node	**head;

	head = get_gc_head_instance();
	current = *head;
	while (current != NULL)
	{
		tmp = current;
		current = current->next;
		free(tmp->pt);
		free(tmp);
	}
	*head = NULL;
}
