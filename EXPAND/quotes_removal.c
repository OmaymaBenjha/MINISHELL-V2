/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_removal.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oben-jha <oben-jha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 23:37:45 by oben-jha          #+#    #+#             */
/*   Updated: 2025/07/11 23:37:45 by oben-jha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static void	clean_quotes_in_args(t_command *cmd)
{
	int		i;
	char	*stripped;

	i = 0;
	if (!cmd || !cmd->args)
		return ;
	while (cmd->args[i])
	{
		stripped = strip_quotes(cmd->args[i]);
		if (stripped)
		{
			unlock_quotes(stripped);
			cmd->args[i] = stripped;
		}
		i++;
	}
}
static void	clean_quotes_in_redirs(t_command *cmd)
{
	t_redir	*redir;
	char	*stripped;

	if (!cmd)
		return ;
	redir = cmd->redirections;
	while (redir)
	{
		if (redir->type != REDIR_HEREDOC)
		{
			stripped = strip_quotes(redir->del_or_fname);
			if (stripped)
				redir->del_or_fname = stripped;
		}
		redir = redir->next;
	}
}

void	quote_remover(t_command *cmd_list)
{
	while (cmd_list)
	{
		clean_quotes_in_args(cmd_list);
		clean_quotes_in_redirs(cmd_list);
		cmd_list = cmd_list->next_piped_command;
	}
}
