/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aziane <aziane@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 21:42:03 by aziane            #+#    #+#             */
/*   Updated: 2025/08/11 21:42:03 by aziane           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../parsing.h"
#include "execution.h"
#include <stdlib.h>

void	free_env(char **envp)
{
	int	i;

	if (!envp)
		return ;
	i = 0;
	while (envp[i])
	{
		free(envp[i]);
		i++;
	}
	free(envp);
}

int	set_env(const char *name, const char *value, t_shell *shell)
{
	int		i;
	size_t	name_len;
	char	*new_var;

	new_var = create_env_string(name, value);
	if (!new_var)
		return (1);
	name_len = ft_strlen(name);
	i = 0;
	while (shell->envp[i])
	{
		if (ft_strncmp(shell->envp[i], name, name_len) == 0)
		{
			free(shell->envp[i]);
			shell->envp[i] = new_var;
			return (0);
		}
		i++;
	}
	return (add_new_env_var(new_var, shell, i));
}

static int	count_valued_vars(char **envp)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (envp[i])
	{
		if (ft_strchr(envp[i], '='))
			count++;
		i++;
	}
	return (count);
}

char	**get_filtered_envp(char **envp)
{
	int		i;
	int		j;
	int		count;
	char	**filtered_envp;

	count = count_valued_vars(envp);
	filtered_envp = malloc(sizeof(char *) * (count + 1));
	if (!filtered_envp)
		return (NULL);
	i = 0;
	j = 0;
	while (envp[i])
	{
		if (ft_strchr(envp[i], '='))
			filtered_envp[j++] = envp[i];
		i++;
	}
	filtered_envp[j] = NULL;
	return (filtered_envp);
}
