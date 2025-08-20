/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aziane <aziane@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 17:17:13 by aziane            #+#    #+#             */
/*   Updated: 2025/08/16 17:17:13 by aziane           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

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

static char	**dup_env_for_sort(char **envp, int *count)
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

static int	show_vars(char **sorted_envp, int i)
{
	char	*equal_sign;
	int		name_len;

	if (!sorted_envp)
		return (1);
	bubble_sort_env(sorted_envp, i);
	i = -1;
	while (sorted_envp && sorted_envp[++i])
	{
		if (ft_strncmp(sorted_envp[i], "_=", 2) == 0
			|| ft_strncmp(sorted_envp[i], "_\0", 2) == 0)
			continue ;
		ft_putstr_fd("declare -x ", 1);
		equal_sign = ft_strchr(sorted_envp[i], '=');
		if (equal_sign)
		{
			name_len = equal_sign - sorted_envp[i];
			(write(1, sorted_envp[i], name_len), ft_putstr_fd("=\"", 1));
			(ft_putstr_fd(equal_sign + 1, 1), ft_putstr_fd("\"", 1));
		}
		else
			ft_putstr_fd(sorted_envp[i], 1);
		ft_putstr_fd("\n", 1);
	}
	return (free(sorted_envp), 0);
}

static int	handle_export_arg(char *arg, t_shell *shell)
{
	char	*equal_sign;
	size_t	arg_len;

	equal_sign = ft_strchr(arg, '=');
	if (ft_strncmp(arg, "_=", 2) == 0 || ft_strncmp(arg, "_\0", 2) == 0)
		return (0);
	if (equal_sign)
	{
		if (!add_or_app_var(arg, equal_sign, shell))
			return (1);
	}
	else
	{
		arg_len = ft_strlen(arg);
		if (!add_without_value(arg, shell, arg_len))
			return (1);
	}
	return (0);
}

int	ft_export(char **args, t_shell *shell)
{
	int		i;
	int		status;
	int		count;

	i = 1;
	status = 0;
	if (!args[1])
		return (show_vars(dup_env_for_sort(shell->envp, &count), count));
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
			if (handle_export_arg(args[i], shell) != 0)
				status = 1;
		i++;
	}
	return (status);
}
