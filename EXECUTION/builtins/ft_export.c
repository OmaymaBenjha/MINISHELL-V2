#include "builtins.h"

static int	is_valid_identifier(const char *s)
{
	int	i;

	if (!s || (!ft_isalpha(s[0]) && s[0] != '_'))
		return (0);
	i = 1;
	while (s[i] && s[i] != '=' && s[i] != '+')
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

static int	handle_export_arg(char *arg, t_shell *shell)
{
	char	*name;
	char	*value;
	char	*equal_sign;
	char	*existing_value;
	char	*new_value;

	equal_sign = ft_strchr(arg, '=');
	if (equal_sign && equal_sign > arg && *(equal_sign - 1) == '+')
	{
		name = ft_substr(arg, 0, equal_sign - arg - 1);
		if (!name)
			return (1);
		value = equal_sign + 1;
		existing_value = my_getenv(name, shell->envp);
		if (existing_value)
		{
			new_value = ft_strjoin(existing_value, value);
			if (!new_value)
				return (free(name), 1);
			set_env(name, new_value, shell);
			free(new_value);
		}
		else
			set_env(name, value, shell);
		return (free(name), 0);
	}
	else if (equal_sign)
	{
		name = ft_substr(arg, 0, equal_sign - arg);
		if (!name)
			return (1);
		value = equal_sign + 1;
		set_env(name, value, shell);
		return (free(name), 0);
	}
	else
	{
		int i = 0;
		size_t arg_len = ft_strlen(arg);
		while (shell->envp[i])
		{
			if (ft_strncmp(shell->envp[i], arg, arg_len) == 0 &&
				(shell->envp[i][arg_len] == '=' || shell->envp[i][arg_len] == '\0'))
				return (0);
			i++;
		}
		int count = i;
		char **new_envp = malloc(sizeof(char *) * (count + 2));
		if (!new_envp)
			return (1);
		i = 0;
		while (i < count)
		{
			new_envp[i] = shell->envp[i];
			i++;
		}
		new_envp[i] = ft_strdup(arg);
		if (!new_envp[i])
			return (free(new_envp), 1);
		new_envp[i + 1] = NULL;
		free(shell->envp);
		shell->envp = new_envp;
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
		if (!is_valid_identifier(args[i]))
		{
			ft_putstr_fd("minishell: export: `", 2);
			ft_putstr_fd(args[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			status = 1;
		}
		else
		{
			if (handle_export_arg(args[i], shell) != 0)
			{
				ft_putstr_fd("minishell: export: allocation error\n", 2);
				status = 1;
			}
		}
		i++;
	}
	return (status);
}