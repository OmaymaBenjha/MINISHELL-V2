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
	if (errno == EACCES)
	{
		(ft_putstr_fd("minishell: ", 2), ft_putstr_fd(cmd->args[0], 2));
		(ft_putstr_fd(": ", 2), ft_putstr_fd("command not found", 2));
		(ft_putstr_fd("\n", 2), exit(127));
	}
	if (errno != ENOEXEC)
	{
		// printf("hre\n");
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->args[0], 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
	}
	else if (errno == ENOEXEC)
        exit(0);
	exit(1);
}
static void	child_process_pipeline(t_command *cmd, t_shell *shell,
		int in_fd, int *pipe_fd)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);

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
	if (handle_redirections(cmd) == -1)
		exit(1);
	exec_command(cmd, shell);
}
static void	parent_handle_pipe_fds(int *in_fd, int *pipe_fd, t_command *cmd)
{
	if (*in_fd != STDIN_FILENO)
		close(*in_fd);
	if (cmd->next_piped_command)
	{
		close(pipe_fd[1]);
		*in_fd = pipe_fd[0];
	}
}

static void execute_pipeline(t_command *cmd, t_shell *shell)
{
    int   pipe_fd[2];
    int   in_fd;
    pid_t pid;

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
                return;
            }
        }
        pid = fork();
        if (pid == -1)
        {
            perror("minishell: fork");
            shell->last_exit_status = 1;
            close(in_fd);
            if (cmd->next_piped_command)
                (close(pipe_fd[0]),  close(pipe_fd[1]));
            wait_for_children(-1);
            return;
        }
        if (pid == 0)
            child_process_pipeline(cmd, shell, in_fd, pipe_fd);
        parent_handle_pipe_fds(&in_fd, pipe_fd, cmd);
        cmd = cmd->next_piped_command;
    }
    shell->last_exit_status = wait_for_children(pid);
}

void	executor(t_command *commands, t_shell *shell)
{
	pid_t	pid;
	int		original_fds[2];

	if (!commands)
		return ;
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	if (commands->next_piped_command)
		execute_pipeline(commands, shell);
	else
	{
		if (commands->args && commands->args[0] && is_parent_builtin(commands->args[0]))
		{
			original_fds[0] = dup(STDIN_FILENO);
			original_fds[1] = dup(STDOUT_FILENO);
			if (handle_redirections(commands) == -1)
			{
				shell->last_exit_status = 1;
				restore_fds(original_fds[0], original_fds[1]);
				return ;
			}
			execute_builtin(commands, shell);
			restore_fds(original_fds[0], original_fds[1]);
			return ;
		}
		pid = fork();
		if (pid == -1)
		{
			shell->last_exit_status = 1;
			return;
		}
		if (pid == 0)
			run_child(commands, shell);
		shell->last_exit_status = wait_for_children(pid);
	}
}
