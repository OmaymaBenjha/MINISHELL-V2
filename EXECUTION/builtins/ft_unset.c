/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oben-jha <oben-jha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 13:17:11 by oben-jha          #+#    #+#             */
/*   Updated: 2025/08/11 13:17:11 by oben-jha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	size_t	len;

	len = ft_strlen(var_to_unset);
	count = count_vars(*envp_ptr);
	new_envp = malloc(sizeof(char *) * (count + 1));
	if (!new_envp)
		return (1);
	i = 0;
	j = 0;
	while (i < count)
	{
		if (ft_strncmp((*envp_ptr)[i], var_to_unset, len) == 0
			&& ((*envp_ptr)[i][len] == '=' || (*envp_ptr)[i][len] == '\0'))
			free((*envp_ptr)[i]);
		else
			new_envp[j++] = (*envp_ptr)[i];
		i++;
	}
	new_envp[j] = NULL;
	*envp_ptr = new_envp;
	return (free(*envp_ptr), 0);
}

int	ft_unset(char **args, t_shell *shell)
{
	int	i;
	int	status;

	i = 1;
	status = 0;
	while (args[i])
	{
		if (is_valid_identifier(args[i]))
			remove_env_var(args[i], &shell->envp);
		else
		{
			ft_putstr_fd("minishell: export: `", 2);
			ft_putstr_fd(args[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			status = 1;
		}
		i++;
	}
	return (status);
}
