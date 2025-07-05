#include "parsing.h"

t_token	*tokenizer(char *line)
{
	t_token	*tokens;
	t_token	*new_token;
	int		i;

	tokens = NULL;
	i = 0;
	while (line[i])
	{
		if (ft_isspace(line[i]))
		{
			i++;
			continue ;
		}
		if (is_metachar(line[i]))
			new_token = get_operator_token(line, &i);
		else
			new_token = get_word_token(line, &i);
		if (new_token == NULL)
			return (NULL);
		add_token_back(&tokens, new_token);
	}
	add_token_back(&tokens, create_token(TOKEN_EOF, NULL));
	return (tokens);
}