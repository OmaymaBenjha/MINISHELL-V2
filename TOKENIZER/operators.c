#include "parsing.h"

t_token	*get_operator_token(char *line, int *i)
{
	if (line[*i] == '|')
	{
		(*i)++;
		return (create_token(TOKEN_PIPE, "|"));
	}
	if (line[*i] == '>')
	{
		(*i)++;
		if (line[*i] == '>')
		{
			(*i)++;
			return (create_token(TOKEN_REDIR_APPEND, ">>"));
		}
		return (create_token(TOKEN_REDIR_OUT, ">"));
	}
	if (line[*i] == '<')
	{
		(*i)++;
		if (line[*i] == '<')
		{
			(*i)++;
			return (create_token(TOKEN_REDIR_HEREDOC, "<<"));
		}
		return (create_token(TOKEN_REDIR_IN, "<"));
	}
	return (NULL);
}
