#include "parsing.h"

static bool	is_single_quoted(const char *str)
{
	size_t	len;

	if (!str)
		return (false);
	len = ft_strlen(str);
	if (len < 2)
		return (false);
	if (str[0] == '\'' && str[len - 1] == '\'')
		return (true);
	return (false);
}

void	global_expand(t_command *cmds_head, t_shell *shell)
{
	t_command	*cmd;
	t_redir		*redir;
	int			i;

	cmd = cmds_head;
	while (cmd)
	{
		i = 0;
		while (cmd->args && cmd->args[i])
		{
			if (!is_single_quoted(cmd->args[i]))
				cmd->args[i] = expander(cmd->args[i], shell);
			i++;
		}
		redir = cmd->redirections;
		while (redir)
		{
			if (redir->type != REDIR_HEREDOC
				&& !is_single_quoted(redir->delimiter_or_filename))
				redir->delimiter_or_filename = expander(redir->delimiter_or_filename,
						shell);
			redir = redir->next;
		}
		cmd = cmd->next_piped_command;
	}
}
