/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oben-jha <oben-jha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/08 13:06:14 by oben-jha          #+#    #+#             */
/*   Updated: 2025/08/08 13:09:42 by oben-jha         ###   ########.fr       */
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
