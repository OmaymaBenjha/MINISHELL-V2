#include "builtins.h"

int	ft_pwd(t_shell *shell)
{
	char	*pwd_val;

	pwd_val = my_getenv("PWD", shell->envp);
	if (pwd_val)
	{
		ft_putstr_fd(pwd_val, 1);
		ft_putstr_fd("\n", 1);
		return (0);
	}
	ft_putstr_fd("minishell: pwd: PWD not set\n", 2);
	return (1);
}