/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aziane <aziane@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 17:21:14 by aziane            #+#    #+#             */
/*   Updated: 2025/08/16 17:21:14 by aziane           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	handle_exit(char *id)
{
	if (errno == EACCES)
	{
		(ft_putstr_fd("minishell: ", 2), ft_putstr_fd(id, 2));
		(ft_putstr_fd(": ", 2), ft_putstr_fd("command not found", 2));
		(ft_putstr_fd("\n", 2), exit(127));
	}
	if (errno != ENOEXEC)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(id, 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
	}
	else if (errno == ENOEXEC)
		exit(0);
}
