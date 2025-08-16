/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aziane <aziane@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 17:17:00 by aziane            #+#    #+#             */
/*   Updated: 2025/08/16 17:17:00 by aziane           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

int	ft_env(t_shell *shell)
{
	int	i;

	i = 0;
	if (!shell->envp)
		return (0);
	while (shell->envp[i])
	{
		if (ft_strchr(shell->envp[i], '='))
		{
			ft_putstr_fd(shell->envp[i], 1);
			ft_putstr_fd("\n", 1);
		}
		i++;
	}
	return (0);
}
