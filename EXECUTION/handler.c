/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aziane <aziane@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 20:09:18 by aziane            #+#    #+#             */
/*   Updated: 2025/08/11 20:56:36 by aziane           ###   ########.fr       */
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

void handler(int sig)
{
	if (sig == SIGINT)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
	}
	else
	{
		write(1, "Quit: 3\n", 9);
		rl_on_new_line();
		rl_replace_line("", 0);
	}
}

