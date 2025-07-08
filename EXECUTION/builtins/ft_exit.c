#include "builtins.h"

int	ft_exit(char **args)
{
	ft_putstr_fd("exit\n", 1);
	if (args[1])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		return (1);
	}
	exit(get_exit_status());
	return (0);
}
