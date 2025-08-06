/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processor.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oben-jha <oben-jha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 23:13:35 by oben-jha          #+#    #+#             */
/*   Updated: 2025/07/11 23:13:35 by oben-jha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static int	count_args_in_segment(t_token *tokens, int *hc)
{
	int	count;

	count = 0;
	while (tokens && tokens->type != TOKEN_PIPE && tokens->type != TOKEN_EOF)
	{
		if (tokens->type == TOKEN_WORD)
			count++;
		else if (tokens->type >= TOKEN_REDIR_IN)
		{
			if (tokens->type == TOKEN_REDIR_HEREDOC)
				(*hc)++;
			tokens = tokens->next;
		}
			
		if (tokens)
			tokens = tokens->next;
	}
	return (count);
}

static int	process_cmds(t_command **command_list, t_token **tokens, int *hc)
{
	t_command	*current_cmd;
	int			i;

	current_cmd = create_command_node();
	add_command_node_back(command_list, current_cmd);
	current_cmd->args = gc_mall(sizeof(char *)
			* (count_args_in_segment(*tokens, hc) + 1));
	i = 0;
	while (*tokens && (*tokens)->type != TOKEN_PIPE
		&& (*tokens)->type != TOKEN_EOF)
	{
		if ((*tokens)->type == TOKEN_WORD)
		{
			current_cmd->args[i++] = gc_strdup((*tokens)->value);
			*tokens = (*tokens)->next;
		}
		else if ((*tokens)->type >= TOKEN_REDIR_IN)
		{
			if (!handle_redirection(current_cmd, tokens))
				return (0);
		}
	}
	current_cmd->args[i] = NULL;
	return (1);
}

t_command	*parser(t_token *tokens)
{
	t_command	*command_list;
	int			here_doc_c;

	here_doc_c = 0;
	if (!tokens || tokens->type == TOKEN_EOF)
		return (NULL);
	command_list = NULL;
	while (tokens && tokens->type != TOKEN_EOF)
	{
		if (tokens->type == TOKEN_PIPE)
			return (syntax_error_handler(tokens->value), NULL);
		if (!process_cmds(&command_list, &tokens, &here_doc_c))
			return (NULL);
		if (tokens && tokens->type == TOKEN_PIPE)
		{
			tokens = tokens->next;
			if (!tokens || tokens->type == TOKEN_EOF)
				return (syntax_error_handler("newline"), NULL);
		}
	}
	if (here_doc_c > 16)
		(heredoc_error(), exit(2));
	return (command_list);
}
