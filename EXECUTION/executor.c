#include "execution.h"

static void	run_child(t_command *cmd, char **envp)
{
	char	*path;

	if (handle_redirections(cmd) == -1)
		exit(1);
	if (!cmd->args || !cmd->args[0])
		exit(0);
	if (is_builtin(cmd->args[0]))
	{
		execute_builtin(cmd, &envp);
		exit(get_exit_status());
	}
	path = find_path(cmd->args[0], envp);
	if (!path)
		exit(127);
	execve(path, cmd->args, envp);
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd->args[0], 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(strerror(errno), 2);
	ft_putstr_fd("\n", 2);
	if (errno == EACCES)
		exit(126);
	exit(1);
}

static void	execute_single_command(t_command *cmd, char ***envp_ptr)
{
	pid_t	pid;
	int		original_fds[2];

	if (cmd->args && cmd->args[0] && is_parent_builtin(cmd->args[0]))
	{
		original_fds[0] = dup(STDIN_FILENO);
		original_fds[1] = dup(STDOUT_FILENO);
		if (handle_redirections(cmd) == -1)
		{
			set_exit_status(1);
			restore_fds(original_fds[0], original_fds[1]);
			return ;
		}
		execute_builtin(cmd, envp_ptr);
		restore_fds(original_fds[0], original_fds[1]);
		return ;
	}
	pid = fork();
	if (pid == -1)
	{
		set_exit_status(1);
		return ;
	}
	if (pid == 0)
		run_child(cmd, *envp_ptr);
	set_exit_status(wait_for_children(pid));
}

static void	child_process_pipeline(t_command *cmd, char **envp,
	int in_fd, int *pipe_fd)
{
	if (in_fd != STDIN_FILENO)
	{
		dup2(in_fd, STDIN_FILENO);
		close(in_fd);
	}
	if (cmd->next_piped_command)
	{
		close(pipe_fd[0]);
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[1]);
	}
	run_child(cmd, envp);
}

static void	execute_pipeline(t_command *cmd, char ***envp_ptr)
{
	int		pipe_fd[2];
	int		in_fd;
	pid_t	pid;

	in_fd = STDIN_FILENO;
	while (cmd)
	{
		if (cmd->next_piped_command)
			if (pipe(pipe_fd) == -1)
				return (set_exit_status(1));
		pid = fork();
		if (pid == -1)
			return (set_exit_status(1));
		if (pid == 0)
			child_process_pipeline(cmd, *envp_ptr, in_fd, pipe_fd);
		if (in_fd != STDIN_FILENO)
			close(in_fd);
		if (cmd->next_piped_command)
		{
			close(pipe_fd[1]);
			in_fd = pipe_fd[0];
		}
		cmd = cmd->next_piped_command;
	}
	set_exit_status(wait_for_children(pid));
}

void	executor(t_command *commands, char ***envp)
{
	if (!commands)
		return ;
	if (commands->next_piped_command)
		execute_pipeline(commands, envp);
	else
		execute_single_command(commands, envp);
}
