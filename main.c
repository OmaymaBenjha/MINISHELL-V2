/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oben-jha <oben-jha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 10:23:15 by oben-jha          #+#    #+#             */
/*   Updated: 2025/08/11 13:42:14 by oben-jha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include "EXECUTION/execution.h"
#include <termios.h>
#include <stdlib.h>

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

static void	setup_signals(void)
{
	struct sigaction	sa;

	g_signal_received = 0;
	rl_catch_signals = 0;
	sa.sa_handler = signal_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}

static void	initialize_shell(t_shell *shell, char **envp)
{
	char	cwd_buffer[1024];

	shell->envp = dupenv(envp);
	shell->last_exit_status = 0;
	if (getcwd(cwd_buffer, sizeof(cwd_buffer)) != NULL)
	{
		set_env("PWD", cwd_buffer, shell);
	}
	else
	{
		perror("minishell: startup error");
		exit(1);
	}
	set_env("OLDPWD", (const char *)NULL, shell);
	set_env("PATH", "/.brew/bin:/mnt/homes/oben-jha/.docker/bin"
		":/usr/gnu/bin:/usr/local/bin:/bin:/usr/bin:.", shell);
	set_env("_", "/usr/bin/env", shell);
	setup_signals();
}

static void	run_parsed_commands(t_command *commands, t_shell *shell)
{
	struct sigaction	sa;

	if (process_heredoc_pipe(commands, shell))
	{
		if (main_expand(commands, shell))
		{
			quote_remover(commands);
			executor(commands, shell);
		}
		else
			shell->last_exit_status = 1;
	}
	sa.sa_handler = signal_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
}

static void	process_input_line(char *line, t_shell *shell)
{
	t_token		*tokens;
	t_command	*commands;

	if (*line)
		add_history(line);
	tokens = tokenizer(line, &shell->last_exit_status);
	commands = parser(tokens, &shell->last_exit_status);
	if (commands)
		run_parsed_commands(commands, shell);
	free(line);
	gc_freed();
}

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
	if (!envp || !isatty(1) || !isatty(0))
		return (1);
	initialize_shell(&shell, envp);
	tcgetattr(STDIN_FILENO, &term);
	shell_loop(&shell, &term);
	free_env(shell.envp);
	gc_freed();
	return (shell.last_exit_status);
}