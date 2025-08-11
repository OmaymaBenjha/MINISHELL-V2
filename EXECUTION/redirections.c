/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aziane <aziane@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 20:45:19 by aziane            #+#    #+#             */
/*   Updated: 2025/08/11 20:45:19 by aziane           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static int	handle_input_redir(t_redir *redir)
{
	int	fd;

	if (redir->type == REDIR_INPUT)
	{
		fd = open(redir->del_or_fname, O_RDONLY);
		if (fd == -1)
		{
			ft_putstr_fd("minishell: ", 2);
			perror(redir->del_or_fname);
			return (-1);
		}
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	else
	{
		dup2(redir->heredoc_fd, STDIN_FILENO);
		close(redir->heredoc_fd);
	}
	return (0);
}

static int	handle_output_redir(t_redir *redir)
{
	int	fd;

	if (redir->type == REDIR_OUTPUT_TRUNC)
		fd = open(redir->del_or_fname,
				O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		fd = open(redir->del_or_fname,
				O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(redir->del_or_fname);
		return (-1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

int	handle_redirections(t_command *cmd)
{
	t_redir	*redir;

	redir = cmd->redirections;
	while (redir)
	{
		if (redir->type == REDIR_INPUT || redir->type == REDIR_HEREDOC)
		{
			if (handle_input_redir(redir) == -1)
				return (-1);
		}
		else if (redir->type == REDIR_OUTPUT_TRUNC
			|| redir->type == REDIR_OUTPUT_APPEND)
		{
			if (handle_output_redir(redir) == -1)
				return (-1);
		}
		redir = redir->next;
	}
	return (0);
}

void	restore_fds(int original_stdin, int original_stdout)
{
	dup2(original_stdin, STDIN_FILENO);
	dup2(original_stdout, STDOUT_FILENO);
	close(original_stdin);
	close(original_stdout);
}

int	wait_for_children(pid_t last_pid)
{
	int	status;
	int	exit_status;
	int	term_sig;

	if (last_pid > 0)
	{
		waitpid(last_pid, &status, 0);
		if (WIFEXITED(status))
			exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
		{
			term_sig = WTERMSIG(status);
			exit_status = 128 + term_sig;
			if (term_sig == SIGQUIT)
				ft_putstr_fd("Quit\n", 2);
		}
	}
	else
		exit_status = 1;
	while (wait(NULL) != -1 || errno != ECHILD)
	{
	}
	return (exit_status);
}
