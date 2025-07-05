
#include "parsing.h"

static char	*maybe_expand_line(char *line, bool flag, char **env)
{
	// char	*new_str;
	// char	*original_expander_result;

	if (flag)
		return (expander(line, env));
	else
		return (gc_strdup(line));
}

int	 process_heredoc_pipe(t_command *cmds_head, char **env)
{
	t_command	*cmd;
	t_redir		*redir;
	int			fd[2];
	char		*line;
	char		*processed_line;
	char		*delimiter;

	cmd = cmds_head;
	while (cmd)
	{
		redir = cmd->redirections;
		while (redir)
		{
			if (redir->type == REDIR_HEREDOC)
			{
				if (pipe(fd) == -1)
					return (perror("pipe"), 0);
				delimiter = strip_quotes(redir->delimiter_or_filename);
				while (1)
				{
					line = readline("heredoc> ");
					if (!line || ft_strcmp(line, delimiter) == 0)
					{
						if (line)
							free(line);  
						break ;
					}
					processed_line = maybe_expand_line(line,
							redir->expand_in_heredoc, env);
					write(fd[1], processed_line, ft_strlen(processed_line));
					write(fd[1], "\n", 1);
					free(line);
					// free(processed_line);
				}
				close(fd[1]);
				redir->heredoc_fd = fd[0];
			}
			redir = redir->next;
		}
		cmd = cmd->next_piped_command;
	}
	return (1);
}
