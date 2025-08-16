/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processor.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aziane <aziane@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 17:32:01 by aziane            #+#    #+#             */
/*   Updated: 2025/08/16 17:32:01 by aziane           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static void	child_heredoc_routine(int *pipe_fd, t_redir *redir, t_shell *shell)
{
	char	*line;
	char	*delimiter;
	char	*processed_line;

	signal(SIGINT, heredoc_sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	close(pipe_fd[0]);
	delimiter = final_delim(redir->del_or_fname);
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
		write(pipe_fd[1], processed_line, ft_strlen(processed_line));
		(write(pipe_fd[1], "\n", 1), free(line));
	}
	(close(pipe_fd[1]), exit(0));
}

static int	parent_wait_heredoc(pid_t pid, int *pipe_fd,
	t_redir *redir, t_shell *shell)
{
	int	status;

	close(pipe_fd[1]);
	waitpid(pid, &status, 0);
	redir->heredoc_fd = pipe_fd[0];
	if ((WIFEXITED(status) && WEXITSTATUS(status) == 130)
		|| (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT))
	{
		close(pipe_fd[0]);
		redir->heredoc_fd = -1;
		shell->last_exit_status = 1;
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
	{
		perror("minishell: pipe");
		shell->last_exit_status = 1;
		return (0);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("minishell: fork");
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		shell->last_exit_status = 1;
		return (0);
	}
	if (pid == 0)
		child_heredoc_routine(pipe_fd, redir, shell);
	return (parent_wait_heredoc(pid, pipe_fd, redir, shell));
}

int	process_heredoc_pipe(t_command *cmds_head, t_shell *shell)
{
	t_command			*cmd;
	t_redir				*redir;
	struct sigaction	sa_orig;
	struct sigaction	sa_ign;
	int					ret;

	ret = 1;
	sa_ign.sa_handler = SIG_IGN;
	sigemptyset(&sa_ign.sa_mask);
	sa_ign.sa_flags = 0;
	sigaction(SIGINT, &sa_ign, &sa_orig);
	cmd = cmds_head;
	while (cmd && ret == 1)
	{
		redir = cmd->redirections;
		while (redir && ret == 1)
		{
			if (redir->type == REDIR_HEREDOC)
				if (!handle_single_heredoc(redir, shell))
					ret = 0;
			redir = redir->next;
		}
		cmd = cmd->next_piped_command;
	}
	return (sigaction(SIGINT, &sa_orig, NULL), ret);
}
