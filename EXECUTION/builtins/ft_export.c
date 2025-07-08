#include "builtins.h"

static int	print_exported_vars(char **envp)
{
	int	i;

	i = 0;
	while (envp && envp[i])
	{
		ft_putstr_fd("declare -x ", 1);
		ft_putstr_fd(envp[i], 1);
		ft_putstr_fd("\n", 1);
		i++;
	}
	return (0);
}

static int	add_or_update_env(char *arg, char ***envp_ptr)
{
	int		i;
	int		count;
	char	**new_envp;
	int		len;

	len = 0;
	while (arg[len] && arg[len] != '=')
		len++;
	i = 0;
	while ((*envp_ptr)[i] && ft_strncmp((*envp_ptr)[i], arg, len + 1) != 0)
		i++;
	if ((*envp_ptr)[i])
		(*envp_ptr)[i] = gc_strdup(arg);
	else
	{
		count = 0;
		while ((*envp_ptr)[count])
			count++;
		new_envp = gc_mall(sizeof(char *) * (count + 2));
		i = -1;
		while (++i < count)
			new_envp[i] = (*envp_ptr)[i];
		new_envp[i++] = gc_strdup(arg);
		new_envp[i] = NULL;
		*envp_ptr = new_envp;
	}
	return (0);
}

int	ft_export(char **args, char ***envp_ptr)
{
	int	i;
	int	status;

	i = 1;
	status = 0;
	if (!args[1])
		return (print_exported_vars(*envp_ptr));
	while (args[i])
	{
		if (!ft_strchr(args[i], '='))
		{
			i++;
			continue ;
		}
		add_or_update_env(args[i], envp_ptr);
		i++;
	}
	return (status);
}
