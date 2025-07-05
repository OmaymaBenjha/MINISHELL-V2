#include "parsing.h"

t_token	*create_token(t_token_type type, char *value)
{
	t_token	*new_token;

	new_token = (t_token *)gc_mall(sizeof(t_token));
	new_token->type = type;
	if (value)
		new_token->value = gc_strdup(value);
	else
		new_token->value = NULL;
	new_token->next = NULL;
	return (new_token);
}

void	add_token_back(t_token **list, t_token *new_token)
{
	t_token	*current;

	if (!list || !new_token)
		return;
	if (*list == NULL)
	{
		*list = new_token;
		return;
	}
	current = *list;
	while (current->next != NULL)
	{
		current = current->next;
	}
	current->next = new_token;
}