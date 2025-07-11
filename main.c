#include "parsing.h"
#include "EXECUTION/execution.h"
#include <termios.h>

volatile sig_atomic_t	g_signal_received = 0;

static void	signal_handler(int sig)
{
	g_signal_received = sig;
	
}

static void	handle_signals_in_main_loop(t_shell *shell)
{
	if (g_signal_received == SIGINT)
	{
		g_signal_received = 0;						
		shell->last_exit_status = 130;
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

static void	init_shell(t_shell *shell, char **envp)
{
	shell->envp = dupenv(envp);
	shell->last_exit_status = 0;
	g_signal_received = 0;
	
}

int	main(int argc, char **argv, char **envp)
{
	char		*line;
	t_token		*tokens;
	t_command	*commands;
	t_shell		shell;
	struct termios	term;

	(void)argc;
	(void)argv;
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, signal_handler);
	rl_catch_signals = 0;
	init_shell(&shell, envp);
	tcgetattr(0, &term);
	while (1)
	{
		handle_signals_in_main_loop(&shell);
		line = readline("minishell> ");
		if (!line)
		{
			ft_putstr_fd("exit\n", 1);
			break ;
		}
		if (*line)
			add_history(line);
		tokens = tokenizer(line);
		commands = parser(tokens);
		if (commands)
		{
			if (process_heredoc_pipe(commands, &shell))
			{
				global_expand(commands, &shell);
				quote_remover(commands);
				executor(commands, &shell);
				tcsetattr(0, TCSANOW, &term);
			}
		}
		free(line);
		gc_freed();
	}
	free_env(shell.envp);
	gc_freed();
	return (shell.last_exit_status);
}
