/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aziane <aziane@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 17:29:04 by aziane            #+#    #+#             */
/*   Updated: 2025/08/20 17:29:04 by aziane           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

char	*final_delim(char *delim)
{
	int	i;

	i = 0;
	while (delim[i])
	{
		if (delim[i] == '$')
			break ;
		i++;
	}
	if (delim[i] == '$' && (delim[i + 1] == '\'' || delim[i + 1] == '\"'))
		delim[i] = '\"';
	return (strip_quotes(delim));
}

void	heredoc_sigint_handler(int sig)
{
	(void)sig;
	exit(130);
}

char	*maybe_expand_line(char *line, bool flag, t_shell *shell)
{
	int	exp;

	exp = 0;
	if (flag)
		return (expander(line, shell, &exp));
	else
		return (gc_strdup(line));
}

void	close_heredoc_fds(t_command *cmds)
{
	t_command	*cmd;
	t_redir		*r;

	cmd = cmds;
	while (cmd)
	{
		r = cmd->redirections;
		while (r)
		{
			if (r->type == REDIR_HEREDOC && r->heredoc_fd >= 0)
			{
				close(r->heredoc_fd);
				r->heredoc_fd = -1;
			}
			r = r->next;
		}
		cmd = cmd->next_piped_command;
	}
}
