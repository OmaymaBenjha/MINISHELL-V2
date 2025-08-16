/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_mall.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oben-jha <oben-jha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 15:43:16 by oben-jha          #+#    #+#             */
/*   Updated: 2025/07/14 15:43:16 by oben-jha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parsing.h"

static t_gc_node	**get_gc_head_instance(void)
{
	static t_gc_node	*head;

	return (&head);
}

void	gc_add_pt(void *pt)
{
	t_gc_node	*new_node;
	t_gc_node	**head;

	if (!pt)
		return ;
	head = get_gc_head_instance();
	new_node = (t_gc_node *)malloc(sizeof(t_gc_node));
	if (!new_node)
	{
		perror("malloc failed in garbage collector");
		exit(EXIT_FAILURE);
	}
	new_node->pt = pt;
	new_node->next = *head;
	*head = new_node;
}

char	*gc_strdup(char *str)
{
	char	*new_str;

	if (!str)
		return (NULL);
	new_str = ft_strdup(str);
	if (!new_str)
	{
		perror("strdup failed!");
		exit(EXIT_FAILURE);
	}
	gc_add_pt(new_str);
	return (new_str);
}

char	*gc_substr(char const *s, unsigned int start, size_t len)
{
	char	*new_str;

	if (!s)
		return (NULL);
	new_str = ft_substr(s, start, len);
	if (!new_str)
	{
		perror("substr failed!");
		exit(EXIT_FAILURE);
	}
	gc_add_pt(new_str);
	return (new_str);
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
