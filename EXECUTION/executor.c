/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aziane <aziane@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 20:53:33 by aziane            #+#    #+#             */
/*   Updated: 2025/08/11 20:53:33 by aziane           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static void	run_child(t_command *cmd, t_shell *shell)
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

static void	execute_pipeline(t_command *cmd, t_shell *shell)
{
	int		pipe_fd[2];
	int		in_fd;
	pid_t	pid;

	in_fd = STDIN_FILENO;
	pid = -1;
	while (cmd)
	{
		if (cmd->next_piped_command)
		{
			if (pipe(pipe_fd) == -1)
			{
				perror("minishell: pipe");
				shell->last_exit_status = 1;
				wait_for_children(pid);
				return ;
			}
		}
		pid = execute_pipeline_command(cmd, shell, in_fd, pipe_fd);
		if (pid == -1)
			return ;
		parent_handle_pipe_fds(&in_fd, pipe_fd, cmd);
		cmd = cmd->next_piped_command;
	}
	shell->last_exit_status = wait_for_children(pid);
}

static void	execute_parent_builtin(t_command *cmd, t_shell *shell)
{
	int	org_fd[2];

	org_fd[0] = dup(STDIN_FILENO);
	org_fd[1] = dup(STDOUT_FILENO);
	if (org_fd[0] == -1 || org_fd[1] == -1)
	{
		perror("minishell: dup");
		if (org_fd[0] != -1)
			close(org_fd[0]);
		if (org_fd[1] != -1)
			close(org_fd[1]);
		shell->last_exit_status = 1;
		return ;
	}
	if (handle_redirections(cmd) == -1)
		shell->last_exit_status = 1;
	else
		execute_builtin(cmd, shell);
	restore_fds(org_fd[0], org_fd[1]);
}

static void	execute_single_command(t_command *cmd, t_shell *shell)
{
	pid_t	pid;

	if (cmd->args && cmd->args[0] && is_parent_builtin(cmd->args[0]))
	{
		execute_parent_builtin(cmd, shell);
		return ;
	}
	pid = fork();
	if (pid == -1)
	{
		perror("minishell: fork");
		shell->last_exit_status = 1;
		return ;
	}
	if (pid == 0)
		run_child(cmd, shell);
	shell->last_exit_status = wait_for_children(pid);
}

void	executor(t_command *commands, t_shell *shell)
{
	if (!commands)
		return ;
	signal(SIGINT, handler);
	signal(SIGQUIT, handler);
	if (commands->next_piped_command)
		execute_pipeline(commands, shell);
	else
		execute_single_command(commands, shell);
}
