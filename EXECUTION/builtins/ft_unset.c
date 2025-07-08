#include "builtins.h"

static int	count_vars(char **envp)
{
	int	i;

	i = 0;
	while (envp && envp[i])
		i++;
	return (i);
}

static int	remove_env_var(char *var_to_unset, char ***envp_ptr)
{
	int		i;
	int		j;
	int		count;
	char	**new_envp;
	int		len;

	len = ft_strlen(var_to_unset);
	count = count_vars(*envp_ptr);
	new_envp = gc_mall(sizeof(char *) * count);
	i = 0;
	j = 0;
	while (i < count)
	{
		if (ft_strncmp((*envp_ptr)[i], var_to_unset, len) != 0
			|| (*envp_ptr)[i][len] != '=')
		{
			new_envp[j] = (*envp_ptr)[i];
			j++;
		}
		i++;
	}
	new_envp[j] = NULL;
	*envp_ptr = new_envp;
	return (0);
}

int	ft_unset(char **args, char ***envp_ptr)
{
	int	i;

	i = 1;
	while (args[i])
	{
		remove_env_var(args[i], envp_ptr);
		i++;
	}
	return (0);
}
