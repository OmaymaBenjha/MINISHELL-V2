#include "builtins.h"

int	ft_exit(char **args, t_shell *shell)
{
	ft_putstr_fd("exit\n", 1);
	if (args[1] && args[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		return (1);
	}
	if (args[1])
	{
		// Note: A proper implementation would check if args[1] is numeric.
		// This is a simplified version for the refactor.
		exit(ft_atoi(args[1]));
	}
	exit(shell->last_exit_status);
	return (0); // This line is never reached
}
