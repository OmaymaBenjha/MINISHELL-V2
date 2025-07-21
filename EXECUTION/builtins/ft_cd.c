#include "builtins.h"
#include <unistd.h>
#include <stdio.h>

static int	update_pwd_in_env(t_shell *shell)
{
	char	cwd_buffer[1024];
	char	*old_pwd_val;

	old_pwd_val = my_getenv("PWD", shell->envp);
	if (old_pwd_val)
		set_env("OLDPWD", old_pwd_val, shell);
	if (getcwd(cwd_buffer, sizeof(cwd_buffer)) != NULL)
	{
		return (set_env("PWD", cwd_buffer, shell));
	}
	perror("minishell: cd: getcwd");
	return (1);
}

int	ft_cd(char **args, t_shell *shell)
{
	char	*path;
	char	*old_pwd_path;

	if (args[1] && args[2])
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		return (1);
	}
	path = args[1];
	if (!path)
	{
		path = my_getenv("HOME", shell->envp);
		if (!path)
		{
			ft_putstr_fd("minishell: cd: HOME not set\n", 2);
			return (1);
		}
	}
	else if (ft_strcmp(path, "-") == 0)
	{
		old_pwd_path = my_getenv("OLDPWD", shell->envp);
		if (!old_pwd_path)
		{
			ft_putstr_fd("minishell: cd: OLDPWD not set\n", 2);
			return (1);
		}
		path = old_pwd_path;
		ft_putstr_fd(path, 1);
		ft_putstr_fd("\n", 1);
	}
	if (chdir(path) != 0)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		perror(path);
		return (1);
	}
	update_pwd_in_env(shell);
	return (0);
}
