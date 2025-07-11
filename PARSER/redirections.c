/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oben-jha <oben-jha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 23:15:09 by oben-jha          #+#    #+#             */
/*   Updated: 2025/07/11 23:15:09 by oben-jha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	handle_redirection(t_command *cmd, t_token **current_token_ptr)
{
	t_token	*redir_token;
	t_redir	*new_redir;

	redir_token = *current_token_ptr;
	if (redir_token->next->type != TOKEN_WORD)
	{
		return (syntax_error_handler(redir_token->next->value));
	}
	new_redir = create_redir_node(redir_token->type, redir_token->next->value);
	add_redir_node_back(&cmd->redirections, new_redir);
	*current_token_ptr = (*current_token_ptr)->next->next;
	return (1);
}
