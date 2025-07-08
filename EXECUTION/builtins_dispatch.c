#include "execution.h"
#include "builtins/builtins.h"

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strcmp(cmd, "echo") == 0)
		return (1);
	if (ft_strcmp(cmd, "cd") == 0)
		return (1);
	if (ft_strcmp(cmd, "pwd") == 0)
		return (1);
	if (ft_strcmp(cmd, "export") == 0)
		return (1);
	if (ft_strcmp(cmd, "unset") == 0)
		return (1);
	if (ft_strcmp(cmd, "env") == 0)
		return (1);
	if (ft_strcmp(cmd, "exit") == 0)
		return (1);
	return (0);
}

int	is_parent_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strcmp(cmd, "cd") == 0)
		return (1);
	if (ft_strcmp(cmd, "export") == 0)
		return (1);
	if (ft_strcmp(cmd, "unset") == 0)
		return (1);
	if (ft_strcmp(cmd, "exit") == 0)
		return (1);
	return (0);
}

void	execute_builtin(t_command *cmd, char ***envp_ptr)
{
	char	*cmd_name;
	int		status;

	status = 0;
	if (!cmd || !cmd->args || !cmd->args[0])
		return ;
	cmd_name = cmd->args[0];
	if (ft_strcmp(cmd_name, "echo") == 0)
		status = ft_echo(cmd->args);
	else if (ft_strcmp(cmd_name, "cd") == 0)
		status = ft_cd(cmd->args, *envp_ptr);
	else if (ft_strcmp(cmd_name, "pwd") == 0)
		status = ft_pwd();
	else if (ft_strcmp(cmd_name, "export") == 0)
		status = ft_export(cmd->args, envp_ptr);
	else if (ft_strcmp(cmd_name, "unset") == 0)
		status = ft_unset(cmd->args, envp_ptr);
	else if (ft_strcmp(cmd_name, "env") == 0)
		status = ft_env(*envp_ptr);
	else if (ft_strcmp(cmd_name, "exit") == 0)
		status = ft_exit(cmd->args);
	set_exit_status(status);
}
