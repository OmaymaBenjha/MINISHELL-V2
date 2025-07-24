/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processor.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oben-jha <oben-jha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 23:17:17 by oben-jha          #+#    #+#             */
/*   Updated: 2025/07/24 17:15:00 by Gemini          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static void	heredoc_sigint_handler(int sig)
{
	(void)sig;
	exit(130);
}

static void	child_heredoc_routine(int *pipe_fd, t_redir *redir, t_shell *shell)
{
	char	*line;
	char	*delimiter;
	char	*processed_line;

	signal(SIGINT, heredoc_sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	close(pipe_fd[0]);
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
		if (redir->expand_in_heredoc)
			processed_line = expander(line, shell, 0);
		else
			processed_line = gc_strdup(line);
		write(pipe_fd[1], processed_line, ft_strlen(processed_line));
		write(pipe_fd[1], "\n", 1);
		free(line);
	}
	close(pipe_fd[1]);
	exit(0);
}

static int	parent_wait_heredoc(pid_t pid, int *pipe_fd,
	t_redir *redir, t_shell *shell)
{
	int	status;

	close(pipe_fd[1]);
	waitpid(pid, &status, 0);
	redir->heredoc_fd = pipe_fd[0];
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		close(pipe_fd[0]);
		redir->heredoc_fd = -1;
		shell->last_exit_status = 130;
		ft_putstr_fd("\n", 2);
		return (0);
	}
	return (1);
}

static int	handle_single_heredoc(t_redir *redir, t_shell *shell)
{
	int		pipe_fd[2];
	pid_t	pid;

	if (pipe(pipe_fd) == -1)
		return (perror("pipe"), 0);
	pid = fork();
	if (pid == -1)
		return (perror("fork"), 0);
	if (pid == 0)
		child_heredoc_routine(pipe_fd, redir, shell);
	return (parent_wait_heredoc(pid, pipe_fd, redir, shell));
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