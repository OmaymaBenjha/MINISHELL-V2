#include "builtins.h"

static int	is_valid_identifier(const char *s)
{
	int	i;

	if (!s || (!ft_isalpha(s[0]) && s[0] != '_'))
		return (0);
	i = 1;
	while (s[i] && s[i] != '=')
	{
		if (!ft_isalnum(s[i]) && s[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static void	bubble_sort_env(char **env, int count)
{
	int		i;
	int		j;
	char	*temp;
	int		swapped;

	i = 0;
	while (i < count - 1)
	{
		swapped = 0;
		j = 0;
		while (j < count - i - 1)
		{
			if (ft_strcmp(env[j], env[j + 1]) > 0)
			{
				temp = env[j];
				env[j] = env[j + 1];
				env[j + 1] = temp;
				swapped = 1;
			}
			j++;
		}
		if (swapped == 0)
			break ;
		i++;
	}
}

static char	**duplicate_env_for_sorting(char **envp, int *count)
{
	int		i;
	char	**new_envp;

	*count = 0;
	while (envp && envp[*count])
		(*count)++;
	new_envp = malloc(sizeof(char *) * (*count + 1));
	if (!new_envp)
		return (NULL);
	i = 0;
	while (i < *count)
	{
		new_envp[i] = envp[i];
		i++;
	}
	new_envp[i] = NULL;
	return (new_envp);
}

static int	print_exported_vars(char **envp)
{
	int		i;
	char	*equal_sign;
	int		name_len;
	char	**sorted_envp;
	int		count;

	sorted_envp = duplicate_env_for_sorting(envp, &count);
	if (!sorted_envp)
		return (1);
	bubble_sort_env(sorted_envp, count);
	i = 0;
	while (sorted_envp && sorted_envp[i])
	{
		ft_putstr_fd("declare -x ", 1);
		equal_sign = ft_strchr(sorted_envp[i], '=');
		if (equal_sign)
		{
			name_len = equal_sign - sorted_envp[i];
			write(1, sorted_envp[i], name_len);
			ft_putstr_fd("=\"", 1);
			ft_putstr_fd(equal_sign + 1, 1);
			ft_putstr_fd("\"", 1);
		}
		else
			ft_putstr_fd(sorted_envp[i], 1);
		ft_putstr_fd("\n", 1);
		i++;
	}
	free(sorted_envp);
	return (0);
}

static int	add_new_env_var(char *arg, char ***envp_ptr, int count)
{
	char	**new_envp;
	int		i;

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
	return (0);
}

static int	add_or_update_env(char *arg, char ***envp_ptr)
{
	int		i;
	size_t	len;

	len = 0;
	while (arg[len] && arg[len] != '=')
		len++;
	i = 0;
	while ((*envp_ptr)[i])
	{
		if (ft_strncmp((*envp_ptr)[i], arg, len) == 0
			&& ((*envp_ptr)[i][len] == '=' || (*envp_ptr)[i][len] == '\0'))
		{
			if (arg[len] == '=')
			{
				free((*envp_ptr)[i]);
				(*envp_ptr)[i] = ft_strdup(arg);
				if (!(*envp_ptr)[i])
					return (1);
			}
			return (0);
		}
		i++;
	}
	return (add_new_env_var(arg, envp_ptr, i));
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
		if (!is_valid_identifier(args[i]))
		{
			ft_putstr_fd("minishell: export: `", 2);
			ft_putstr_fd(args[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			status = 1;
		}
		else
		{
			if (add_or_update_env(args[i], &shell->envp) != 0)
			{
				ft_putstr_fd("minishell: export: allocation error\n", 2);
				status = 1;
			}
		}
		i++;
	}
	return (status);
}