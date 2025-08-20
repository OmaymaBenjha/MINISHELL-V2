/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aziane <aziane@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 17:28:03 by aziane            #+#    #+#             */
/*   Updated: 2025/08/20 17:28:03 by aziane           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include <unistd.h>

int	ft_pwd(t_shell *shell)
{
	char	buf[PATH_MAX];

	if (shell->cwd[0] != '\0')
	{
		ft_putstr_fd(shell->cwd, 1);
		ft_putstr_fd("\n", 1);
		return (0);
	}
	if (getcwd(buf, PATH_MAX) != NULL)
	{
		ft_putstr_fd(buf, 1);
		ft_putstr_fd("\n", 1);
		return (0);
	}
	ft_putstr_fd("minishell: pwd: error retrieving current directory: "
		"getcwd: cannot access parent directories\n", 2);
	return (1);
}
