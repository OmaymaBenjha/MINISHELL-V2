#include "parsing.h"

static int	count_args_in_segment(t_token *tokens)
{
	int	count;

	count = 0;
	while (tokens && tokens->type != TOKEN_PIPE && tokens->type != TOKEN_EOF)
	{
		if (tokens->type == TOKEN_WORD)
			count++;
		else if (tokens->type >= TOKEN_REDIR_IN)
			tokens = tokens->next;
		if (tokens)
			tokens = tokens->next;
	}
	return (count);
}

t_command	*parser(t_token *tokens)
{
	t_command	*command_list;
	t_command	*current_cmd;
	t_token		*current_token;
	int			i;

	if (!tokens || tokens->type == TOKEN_EOF)
		return (NULL);
	command_list = NULL;
	current_token = tokens;
	while (current_token && current_token->type != TOKEN_EOF)
	{
		if (current_token->type == TOKEN_PIPE)
			return (syntax_error_handler(current_token->value), NULL);
		current_cmd = create_command_node();
		add_command_node_back(&command_list, current_cmd);
		current_cmd->args = gc_mall(sizeof(char *) * \
			(count_args_in_segment(current_token) + 1));
		i = 0;
		while (current_token && current_token->type != TOKEN_PIPE \
			&& current_token->type != TOKEN_EOF)
		{
			if (current_token->type == TOKEN_WORD)
			{
				current_cmd->args[i++] = gc_strdup(current_token->value);
				current_token = current_token->next;
			}
			else if (current_token->type >= TOKEN_REDIR_IN)
			{
				if (!handle_redirection(current_cmd, &current_token))
					return (NULL);
			}
		}
		current_cmd->args[i] = NULL;
		if (current_token && current_token->type == TOKEN_PIPE)
		{
			current_token = current_token->next;
			if (current_token == NULL || current_token->type == TOKEN_EOF)
				return (syntax_error_handler("newline"), NULL);
		}
	}
	return (command_list);
}
