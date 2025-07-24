#include "builtins.h"

int	ft_env(t_shell *shell)
{
	int	i;

	i = 0;
	if (!shell->envp)
		return (0);
	while (shell->envp[i])
	{
        if (ft_strchr(shell->envp[i], '='))
        {
            ft_putstr_fd(shell->envp[i], 1);
            ft_putstr_fd("\n", 1);
        }
        i++;
	}
	return (0);
}
