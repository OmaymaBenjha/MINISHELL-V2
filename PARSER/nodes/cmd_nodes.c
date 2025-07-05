#include "parsing.h"

t_command	*create_command_node(void)
{
	t_command	*new_cmd;

	new_cmd = (t_command *)gc_mall(sizeof(t_command));
	new_cmd->args = NULL;
	new_cmd->redirections = NULL;
	new_cmd->next_piped_command = NULL;
	return (new_cmd);
}

void	add_command_node_back(t_command **list, t_command *new_cmd)
{
	t_command	*current;

	if (!list || !new_cmd)
		return;
	if (*list == NULL)
	{
		*list = new_cmd;
		return;
	}
	current = *list;
	while (current->next_piped_command != NULL)
		current = current->next_piped_command;
	current->next_piped_command = new_cmd;
}