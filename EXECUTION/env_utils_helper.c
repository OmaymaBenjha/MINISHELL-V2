/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils_helper.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aziane <aziane@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 20:44:03 by aziane            #+#    #+#             */
/*   Updated: 2025/08/11 22:33:23 by aziane           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parsing.h"

static int	count_env_vars(char **envp)
{
	int	i;

	i = 0;
	if (!envp)
		return (0);
	while (envp[i])
		i++;
	return (i);
}

char	**dupenv(char **envp)
{
	int		i;
	int		count;
	char	**new_envp;

	count = count_env_vars(envp);
	new_envp = (char **)malloc(sizeof(char *) * (count + 1));
	if (!new_envp)
		return (NULL);
	i = 0;
	while (i < count)
	{
		new_envp[i] = ft_strdup(envp[i]);
		if (!new_envp[i])
		{
			while (--i >= 0)
				free(new_envp[i]);
			free(new_envp);
			return (NULL);
		}
		i++;
	}
	new_envp[i] = NULL;
	return (new_envp);
}

char	*my_getenv(const char *name, char **env)
{
	int		i;
	size_t	name_len;

	if (!name || !env)
		return (NULL);
	name_len = ft_strlen(name);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], name, name_len) == 0)
		{
			if (env[i][name_len] == '=')
				return (env[i] + name_len + 1);
		}
		i++;
	}
	return (NULL);
}

int	add_new_env_var(char *new_var, t_shell *shell, int size)
{
	char	**new_envp;
	int		i;

	new_envp = malloc(sizeof(char *) * (size + 2));
	if (!new_envp)
	{
		free(new_var);
		return (1);
	}
	i = -1;
	while (shell->envp[++i])
		new_envp[i] = shell->envp[i];
	new_envp[i] = new_var;
	new_envp[i + 1] = NULL;
	free(shell->envp);
	shell->envp = new_envp;
	return (0);
}

char	*create_env_string(const char *name, const char *value)
{
	char	*new_var;
	char	*temp;

	if (value != (char const *) NULL)
	{
		temp = ft_strjoin(name, "=");
		if (!temp)
			return (NULL);
		new_var = ft_strjoin(temp, value);
		free(temp);
		return (new_var);
	}
	else
		return (ft_strdup((char *)name));
}
