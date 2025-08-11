/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oben-jha <oben-jha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 13:16:46 by oben-jha          #+#    #+#             */
/*   Updated: 2025/08/11 13:16:46 by oben-jha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

int	ft_pwd(t_shell *shell)
{
	char	*pwd_val;

	pwd_val = my_getenv("PWD", shell->envp);
	if (pwd_val)
	{
		ft_putstr_fd(pwd_val, 1);
		ft_putstr_fd("\n", 1);
		return (0);
	}
	ft_putstr_fd("minishell: pwd: PWD not set\n", 2);
	return (1);
}
