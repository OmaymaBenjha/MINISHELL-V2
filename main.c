#include "parsing.h"

void	print_commands(t_command *list)
{
	t_command	*cmd;
	t_redir		*redir;
	int			i;
	int			j;

	cmd = list;
	i = 1;
	while (cmd)
	{
		printf("\n--- Command %d ---\n", i++);
		j = 0;
		if (cmd->args)
		{
			while (cmd->args[j])
			{
				printf("  args[%d]: [%s]\n", j, cmd->args[j]);
				j++;
			}
		}
		else
		{
			printf("  args: (null)\n");
		}
		redir = cmd->redirections;
		if (redir)
		{
			j = 1;
			while (redir)
			{
				printf("  Redir %d -> Type: %d, File: [%s]\n", j++, redir->type,
					redir->delimiter_or_filename);
				redir = redir->next;
			}
		}
		cmd = cmd->next_piped_command;
		if (cmd)
		{
			printf("    |\n");
			printf("    V\n");
		}
	}
	printf("\n");
}

int	main(int ac, char **arg, char **env)
{
	char		*input_line;
	t_token		*tokens;
	t_command	*commands;

	(void)ac;
	(void)arg;
	while (1)
	{
		input_line = readline("minishell> ");
		if (!input_line)
		{
			printf("exit\n");
			break ;
		}
		if (input_line[0] != '\0')
		{
			add_history(input_line);
			tokens = tokenizer(input_line);
			if (tokens)
			{
				commands = parser(tokens);
				if (commands)
				{
					// heredoc, in order to expand it , the delimiter shouldn't be quoted at all
					if (process_heredoc_pipe(commands, env))
					{
					// None heredoc cases , i  order to expand it, filenames and args shouln't be 
					// single quoted 
						global_expand(commands, env);
						quote_remover(commands);
						executor(commands, env);
					}
				}
			}
		}
		free(input_line);
		gc_freed();
	}
	gc_freed();
	return (0);
}
