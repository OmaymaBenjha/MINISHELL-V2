/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_nodes.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oben-jha <oben-jha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 23:15:32 by oben-jha          #+#    #+#             */
/*   Updated: 2025/07/11 23:15:32 by oben-jha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static bool	is_quoted(const char *str)
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

t_redir	*create_redir_node(t_token_type type, char *filename)
{
	t_redir	*new_redir;

	new_redir = (t_redir *)gc_mall(sizeof(t_redir));
	if (type == TOKEN_REDIR_IN)
		new_redir->type = REDIR_INPUT;
	else if (type == TOKEN_REDIR_OUT)
		new_redir->type = REDIR_OUTPUT_TRUNC;
	else if (type == TOKEN_REDIR_APPEND)
		new_redir->type = REDIR_OUTPUT_APPEND;
	else if (type == TOKEN_REDIR_HEREDOC)
		new_redir->type = REDIR_HEREDOC;
	new_redir->del_or_fname = gc_strdup(filename);
	new_redir->heredoc_fd = -1;
	new_redir->expand_in_heredoc = !is_quoted(filename);
	new_redir->next = NULL;
	return (new_redir);
}

void	add_redir_node_back(t_redir **list, t_redir *new_redir)
{
	t_redir	*current;

	if (!list || !new_redir)
		return ;
	if (*list == NULL)
	{
		*list = new_redir;
		return ;
	}
	current = *list;
	while (current->next != NULL)
		current = current->next;
	current->next = new_redir;
}
