/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processor.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oben-jha <oben-jha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 23:17:17 by oben-jha          #+#    #+#             */
/*   Updated: 2025/07/12 23:38:40 by oben-jha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static char	*maybe_expand_line(char *line, bool flag, t_shell *shell)
{
	if (flag)
		return (expander(line, shell, 0));
	else
		return (gc_strdup(line));
}

static int	handle_single_heredoc(t_redir *redir, t_shell *shell)
{
	int		fd[2];
	char	*line;
	char	*processed_line;
	char	*delimiter;

	if (pipe(fd) == -1)
		return (perror("pipe"), 0);
	delimiter = strip_quotes(redir->del_or_fname);
	while (1)
	{
		line = readline("heredoc> ");
		if (!line || ft_strcmp(line, delimiter) == 0)
		{
			if (line)
				free(line);
			break ;
		}
		processed_line = maybe_expand_line(line,
				redir->expand_in_heredoc, shell);
		write(fd[1], processed_line, ft_strlen(processed_line));
		(write(fd[1], "\n", 1), free(line));
	}
	close(fd[1]);
	redir->heredoc_fd = fd[0];
	return (1);
}

int	process_heredoc_pipe(t_command *cmds_head, t_shell *shell)
{
	t_command	*cmd;
	t_redir		*redir;

	cmd = cmds_head;
	while (cmd)
	{
		redir = cmd->redirections;
		while (redir)
		{
			if (redir->type == REDIR_HEREDOC)
			{
				if (!handle_single_heredoc(redir, shell))
					return (0);
			}
			redir = redir->next;
		}
		cmd = cmd->next_piped_command;
	}
	return (1);
}
