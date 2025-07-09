#include "parsing.h"
#include "EXECUTION/execution.h"
#include <termios.h>

static void	signal_handler(int sig)
{
	if (sig == SIGINT)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	else if (sig == SIGQUIT)
	{
		(void)sig;
	}
}

int	main(int argc, char **argv, char **envp)
{
	char		*line;
	char		**env_copy;
	t_token		*tokens;
	t_command	*commands;
	struct termios term;
	(void)argc;
	(void)argv;
		rl_catch_signals = 0;

	env_copy = dupenv(envp);
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, signal_handler);
	tcgetattr(0, &term);
	while (1)
	{
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
			if (process_heredoc_pipe(commands, env_copy))
			{
				global_expand(commands, env_copy);
				quote_remover(commands);
				executor(commands, &env_copy);
				tcsetattr(0, TCSANOW, &term);
			}
		}
		free(line);
		gc_freed();
	}
	free_env(env_copy);
	gc_freed();
	return (get_exit_status());
}
