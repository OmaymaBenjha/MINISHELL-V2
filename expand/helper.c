/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oben-jha <oben-jha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 10:57:02 by oben-jha          #+#    #+#             */
/*   Updated: 2025/08/11 08:47:47 by oben-jha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	flag_redir_error(char *exp, char *org, int count)
{
	if (ft_strchr(exp, '$') && (count == 1))
		return (ft_putstr_fd("minishell: : No such file or directory\n", 2), 0);
	if (count != 1)
	{
		(ft_putstr_fd("minishell: ", 2), ft_putstr_fd(org, 2));
		return (ft_putstr_fd(": ambiguous redirect\n", 2), 0);
	}
	return (1);
}

void	add_arg_to_list(t_arg_list **head, char *arg_val)
{
	t_arg_list	*new_node;
	t_arg_list	*current;

	new_node = gc_mall(sizeof(t_arg_list));
	new_node->arg = arg_val;
	new_node->next = NULL;
	if (!*head)
	{
		*head = new_node;
		return ;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = new_node;
}

char	**convert_list_to_array(t_arg_list *head)
{
	int			count;
	t_arg_list	*current;
	char		**array;
	int			i;

	count = 0;
	current = head;
	while (current)
	{
		count++;
		current = current->next;
	}
	array = gc_mall(sizeof(char *) * (count + 1));
	current = head;
	i = 0;
	while (current)
	{
		array[i++] = current->arg;
		current = current->next;
	}
	array[i] = NULL;
	return (array);
}

void	init_data(t_exp_data **data_ptr, t_shell *shell, int *exp)
{
	*data_ptr = gc_mall(sizeof(t_exp_data));
	if (!(*data_ptr))
		return ;
	(*data_ptr)->shell = shell;
	(*data_ptr)->flag = exp;
	(*data_ptr)->next = NULL;
}
