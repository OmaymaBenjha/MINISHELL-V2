/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oben-jha <oben-jha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 19:42:07 by oben-jha          #+#    #+#             */
/*   Updated: 2025/07/13 21:57:09 by oben-jha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include "EXECUTION/execution.h"
#include <termios.h>

volatile sig_atomic_t	g_signal_received = 0;

static void	signal_handler(int sig)
{
	g_signal_received = sig;
	if (sig == SIGINT)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

static void	initialize_shell(t_shell *shell, char **envp)
{
	struct sigaction	sa;

	shell->envp = dupenv(envp);
	shell->last_exit_status = 0;
	g_signal_received = 0;
	rl_catch_signals = 0;
	sa.sa_handler = signal_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}

static void	process_input_line(char *line, t_shell *shell, struct termios *term)
{
	t_token				*tokens;
	t_command			*commands;
	struct sigaction	sa;

	if (*line)
		add_history(line);
	tokens = tokenizer(line);
	commands = parser(tokens);
	if (commands)
	{
		signal(SIGINT, SIG_IGN);
		if (process_heredoc_pipe(commands, shell))
		{
			if (main_expand(commands, shell))
				(quote_remover(commands), executor(commands, shell));
			else
				shell->last_exit_status = 1;
		}
		tcsetattr(0, TCSANOW, term);
		sa.sa_handler = signal_handler;
		sigemptyset(&sa.sa_mask);
		sa.sa_flags = 0;
		sigaction(SIGINT, &sa, NULL);
	}
	(free(line), gc_freed());
}

static void	shell_loop(t_shell *shell, struct termios *term)
{
	char	*line;

	while (1)
	{
		if (g_signal_received == SIGINT)
		{
			shell->last_exit_status = 130;
			g_signal_received = 0;
		}
		line = readline("minishell> ");
		if (!line)
		{
			ft_putstr_fd("exit\n", 1);
			break ;
		}
		process_input_line(line, shell, term);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_shell			shell;
	struct termios	term;

	(void)argc;
	(void)argv;
	initialize_shell(&shell, envp);
	tcgetattr(0, &term);
	shell_loop(&shell, &term);
	free_env(shell.envp);
	gc_freed();
	return (shell.last_exit_status);
}