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
	size_t	len;

	len = 0;
	while (arg[len] && arg[len] != '=')
		len++;
	i = 0;
	while ((*envp_ptr)[i]
		&& (ft_strncmp((*envp_ptr)[i], arg, len) || (*envp_ptr)[i][len] != '='))
		i++;
	if ((*envp_ptr)[i])
	{
		free((*envp_ptr)[i]);
		(*envp_ptr)[i] = ft_strdup(arg);
		if (!(*envp_ptr)[i])
			return (1);
	}
	else
	{
		count = 0;
		while ((*envp_ptr)[count])
			count++;
		new_envp = malloc(sizeof(char *) * (count + 2));
		if (!new_envp)
			return (1);
		i = -1;
		while (++i < count)
			new_envp[i] = (*envp_ptr)[i];
		new_envp[i] = ft_strdup(arg);
		if (!new_envp[i])
		{
			free(new_envp);
			return (1);
		}
		new_envp[i + 1] = NULL;
		free(*envp_ptr);
		*envp_ptr = new_envp;
	}
	return (0);
}

int	ft_export(char **args, t_shell *shell)
{
	int	i;
	int	status;

	i = 1;
	status = 0;
	if (!args[1])
		return (print_exported_vars(shell->envp));
	while (args[i])
	{
		if (!ft_strchr(args[i], '='))
		{
			i++;
			continue ;
		}
		if (add_or_update_env(args[i], &shell->envp) != 0)
		{
			ft_putstr_fd("minishell: export: allocation error\n", 2);
			status = 1;
		}
		i++;
	}
	return (status);
}