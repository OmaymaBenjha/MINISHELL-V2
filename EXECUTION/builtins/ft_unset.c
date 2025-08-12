#include "builtins.h"

static int	find_env_var_index(char **envp, const char *var_to_find)
{
	int		i;
	size_t	len;

	len = ft_strlen(var_to_find);
	i = 0;
	while (envp && envp[i])
	{
		if (ft_strncmp(envp[i], var_to_find, len) == 0
			&& (envp[i][len] == '=' || envp[i][len] == '\0'))
		{
			return (i);
		}
		i++;
	}
	return (-1);
}

static char	**rebuild_env_without_var(char **old_envp, int index_to_skip)
{
	int		count;
	char	**new_envp;
	int		i;
	int		j;

	count = 0;
	while (old_envp[count])
		count++;
	new_envp = malloc(sizeof(char *) * count);
	if (!new_envp)
		return (NULL);
	i = 0;
	j = 0;
	while (i < count)
	{
		if (i == index_to_skip)
			free(old_envp[i]);
		else
			new_envp[j++] = old_envp[i];
		i++;
	}
	new_envp[j] = NULL;
	return (new_envp);
}

static int	remove_env_var(char *var_to_unset, char ***envp_ptr)
{
	int		index_to_remove;
	char	**new_envp;
	char	**old_envp;

	old_envp = *envp_ptr;
	index_to_remove = find_env_var_index(old_envp, var_to_unset);
	if (index_to_remove == -1)
		return (0);
	new_envp = rebuild_env_without_var(old_envp, index_to_remove);
	if (!new_envp)
		return (1);
	*envp_ptr = new_envp;
	free(old_envp);
	return (0);
}

int	ft_unset(char **args, t_shell *shell)
{
	int	i;
	int	status;

	i = 1;
	status = 0;
	if (!args[i])
		return (0);
	while (args[i])
	{
		if (is_valid_identifier(args[i]))
			remove_env_var(args[i], &shell->envp);
		else
		{
			ft_putstr_fd("minishell: unset: `", 2);
			ft_putstr_fd(args[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			status = 1;
		}
		i++;
	}
	return (status);
}
