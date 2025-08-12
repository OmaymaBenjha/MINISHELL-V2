/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aziane <aziane@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 10:23:15 by oben-jha          #+#    #+#             */
/*   Updated: 2025/08/11 22:33:49 by aziane           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include "EXECUTION/execution.h"
#include <termios.h>

volatile sig_atomic_t	g_signal_received = 0;

static void	shell_loop(t_shell *shell, struct termios *term)
{
	char	*line;

	while (1)
	{
		line = readline("minishell> ");
		if (g_signal_received == SIGINT)
		{
			shell->last_exit_status = 1;
			g_signal_received = 0;
		}
		if (!line)
		{
			ft_putstr_fd("exit\n", 1);
			break ;
		}
		process_input_line(line, shell);
		tcsetattr(STDIN_FILENO, TCSANOW, term);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_shell			shell;
	struct termios	term;

	(void)argv;
	if (argc > 1)
	{
		printf("usage : ./minishell\n");
		return (1);
	}
	if (!isatty(1) || !isatty(0))
		return (1);
	initialize_shell(&shell, envp);
	tcgetattr(STDIN_FILENO, &term);
	shell_loop(&shell, &term);
	free_env(shell.envp);
	gc_freed();
	return (shell.last_exit_status);
}
