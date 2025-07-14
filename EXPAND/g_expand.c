/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_expand.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oben-jha <oben-jha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 05:41:03 by oben-jha          #+#    #+#             */
/*   Updated: 2025/07/13 23:30:38 by oben-jha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "parsing.h"

typedef struct s_arg_list
{
	char				*arg;
	struct s_arg_list	*next;
}	t_arg_list;


static void	add_arg_to_list(t_arg_list **head, char *arg_val)
{
	t_arg_list	*new_node;
	t_arg_list	*current;

	new_node = gc_mall(sizeof(t_arg_list));
	new_node->arg = arg_val;
	new_node->next = NULL;
	if (!*head)
	{
		*head = new_node;
		return ;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = new_node;
}

static char	**convert_list_to_array(t_arg_list *head)
{
	int			count;
	t_arg_list	*current;
	char		**array;
	int			i;

	count = 0;
	current = head;
	while (current)
	{
		count++;
		current = current->next;
	}
	array = gc_mall(sizeof(char *) * (count + 1));
	current = head;
	i = 0;
	while (current)
	{
		array[i++] = current->arg;
		current = current->next;
	}
	array[i] = NULL;
	return (array);
}


static int	process_single_redir(t_redir *redir, t_shell *shell)
{
	int		has_expanded;
	char	*original_str;
	char	*expanded_str;
	char	**split_words;
	int		word_count;

	has_expanded = 0;
	original_str = redir->del_or_fname;
	expanded_str = expander(original_str, shell, &has_expanded);
	if ((has_expanded && !ft_strchr(original_str, '\"')
		&& !ft_strchr(original_str, '\'')) || (ft_strlen(expanded_str) == 0))
	{
	
		split_words = ft_split(expanded_str, ' ');
		word_count = 0;
		while (split_words && split_words[word_count])
			word_count++;
		if (word_count != 1)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(original_str, 2);
			ft_putstr_fd(": ambiguous redirect\n", 2);
			return (0);
		}
		redir->del_or_fname = split_words[0];
	}
	else
		redir->del_or_fname = expanded_str;
	return (1);
}


static void	expand_and_split_arg(char *arg, t_shell *shell, t_arg_list **head)
{
	int		has_expanded;
	char	*expanded_str;
	char	**split_words;
	int		j;

	has_expanded = 0;
	expanded_str = expander(arg, shell, &has_expanded);
	if (has_expanded && !ft_strchr(arg, '\"') && !ft_strchr(arg, '\''))
	{
		split_words = ft_split(expanded_str, ' ');
		j = 0;
		while (split_words && split_words[j])
			add_arg_to_list(head, split_words[j++]);
	}
	else
		add_arg_to_list(head, expanded_str);
}



static int	expand_and_check_redirs(t_command *cmd, t_shell *shell)
{
	t_redir	*redir;

	redir = cmd->redirections;
	while (redir)
	{
		if (redir->type != REDIR_HEREDOC)
		{
			if (!process_single_redir(redir, shell))
				return (0);
		}
		redir = redir->next;
	}
	return (1);
}

static void	rebuild_args_with_splitting(t_command *cmd, t_shell *shell)
{
	t_arg_list	*arg_head;
	int			i;

	if (!cmd->args)
		return ;
	arg_head = NULL;
	i = 0;
	while (cmd->args[i])
	{
		expand_and_split_arg(cmd->args[i], shell, &arg_head);
		i++;
	}
	cmd->args = convert_list_to_array(arg_head);
}

int	main_expand(t_command *cmds, t_shell *shell)
{
	t_command	*cmd;

	if (!cmds)
		return (1);
	cmd = cmds;
	while (cmd)
	{
		if (!expand_and_check_redirs(cmd, shell))
			return (0);
		rebuild_args_with_splitting(cmd, shell);
		cmd = cmd->next_piped_command;
	}
	return (1);
}