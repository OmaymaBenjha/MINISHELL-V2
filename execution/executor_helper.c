/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_helper.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aziane <aziane@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 20:41:31 by aziane            #+#    #+#             */
/*   Updated: 2025/08/11 21:25:01 by aziane           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static void	exec_command(t_command *cmd, t_shell *shell)
{
	char	*path;

	if (!cmd->args || !cmd->args[0])
		exit(0);
	if (is_builtin(cmd->args[0]))
	{
		execute_builtin(cmd, shell);
		exit(shell->last_exit_status);
	}
	path = find_path(cmd->args[0], shell->envp);
	if (!path)
		exit(127);
	execve(path, cmd->args, shell->envp);
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd->args[0], 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(strerror(errno), 2);
	ft_putstr_fd("\n", 2);
	if (errno == EACCES)
		exit(126);
	exit(1);
}

static void	child_process_pipeline(t_command *cmd, t_shell *shell,
		int in_fd, int *pipe_fd)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (in_fd != STDIN_FILENO)
	{
		if (dup2(in_fd, STDIN_FILENO) == -1)
		{
			perror("minishell: dup2");
			exit(EXIT_FAILURE);
		}
		close(in_fd);
	}
	if (cmd->next_piped_command)
	{
		close(pipe_fd[0]);
		if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
		{
			perror("minishell: dup2");
			exit(EXIT_FAILURE);
		}
		close(pipe_fd[1]);
	}
	if (handle_redirections(cmd) == -1)
		exit(1);
	exec_command(cmd, shell);
}

pid_t	execute_pipeline_command(t_command *cmd, t_shell *shell,
	int in_fd, int *pipe_fd)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("minishell: fork");
		shell->last_exit_status = 1;
		close(in_fd);
		if (cmd->next_piped_command)
		{
			close(pipe_fd[0]);
			close(pipe_fd[1]);
		}
		wait_for_children(-1);
		return (-1);
	}
	if (pid == 0)
		child_process_pipeline(cmd, shell, in_fd, pipe_fd);
	return (pid);
}

void	parent_handle_pipe_fds(int *in_fd, int *pipe_fd, t_command *cmd)
{
	if (*in_fd != STDIN_FILENO)
		close(*in_fd);
	if (cmd->next_piped_command)
	{
		close(pipe_fd[1]);
		*in_fd = pipe_fd[0];
	}
}

void	run_child(t_command *cmd, t_shell *shell)
{
	char	*path;
	char	**filtered_envp;

	(signal(SIGINT, SIG_DFL), signal(SIGQUIT, SIG_DFL));
	if (handle_redirections(cmd) == -1)
		exit(1);
	if (!cmd->args || !cmd->args[0])
		exit(0);
	if (is_builtin(cmd->args[0]))
		(execute_builtin(cmd, shell), exit(shell->last_exit_status));
	path = find_path(cmd->args[0], shell->envp);
	if (!path)
		exit(127);
	filtered_envp = get_filtered_envp(shell->envp);
	(execve(path, cmd->args, filtered_envp), free(filtered_envp));
	handle_exit(cmd->args[0]);
	exit(1);
}
