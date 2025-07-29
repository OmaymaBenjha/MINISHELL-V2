/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_expand.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oben-jha <oben-jha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 05:41:03 by oben-jha          #+#    #+#             */
/*   Updated: 2025/07/14 15:12:23 by oben-jha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include <stdbool.h>

static bool is_fully_quoted(const char *arg)
{
    size_t len;

    if (!arg)
        return (false);
    len = ft_strlen(arg);
    if (len < 2)
        return (false);
    if (arg[0] == '\"' && arg[len - 1] == '\"')
        return (true);
    if (arg[0] == '\'' && arg[len - 1] == '\'')
        return (true);
    return (false);
}

static bool is_valid_assignment_for_no_split(const char *arg)
{
    char *equal_sign;
    int i;

    if (!arg)
        return (false);
    equal_sign = ft_strchr(arg, '=');
    if (equal_sign == NULL || equal_sign == arg)
        return (false);
    if (!ft_isalpha(arg[0]) && arg[0] != '_')
        return (false);
    i = 1;
    while (&arg[i] < equal_sign)
    {
        if (!ft_isalnum(arg[i]) && arg[i] != '_')
            return (false);
        i++;
    }
    return (true);
}

static void expand_and_split_arg(char *arg, t_shell *shell, t_arg_list **head, bool prevent_splitting)
{
	int		has_expanded;
	char	*exp_str;
	bool	should_split;

	has_expanded = 0;
	exp_str = expander(arg, shell, &has_expanded);

	should_split = has_expanded
		&& !is_fully_quoted(arg)
		&& !prevent_splitting;

	if (should_split)
	{
		char **split_words = ft_split(exp_str, ' ');
		int j = 0;
		if (!split_words || !split_words[0])
			add_arg_to_list(head, gc_strdup(""));
		else
		{
			while (split_words && split_words[j])
				add_arg_to_list(head, split_words[j++]);
		}
	}
	else
	{
		add_arg_to_list(head, exp_str);
	}
}

static void rebuild_args_with_splitting(t_command *cmd, t_shell *shell)
{
	t_arg_list	*arg_head;
	int			i;
	bool		is_literal_export;

	if (!cmd->args || !cmd->args[0])
		return;

	arg_head = NULL;
	is_literal_export = (ft_strcmp(cmd->args[0], "export") == 0);

	i = 0;
	while (cmd->args[i])
	{
		bool prevent_splitting_for_this_arg = false;
		
		if (is_literal_export && i > 0 && is_valid_assignment_for_no_split(cmd->args[i]))
		{
			prevent_splitting_for_this_arg = true;
		}

		expand_and_split_arg(cmd->args[i], shell, &arg_head, prevent_splitting_for_this_arg);
		i++;
	}
	cmd->args = convert_list_to_array(arg_head);
}

static int	process_single_redir(t_redir *redir, t_shell *shell, int *ex)
{
	char	*original_str;
	char	*exp_str;
	char	**split_words;
	int		word_count;

	original_str = redir->del_or_fname;
	exp_str = expander(original_str, shell, ex);
	if (((*ex) && !is_fully_quoted(original_str)) || (ft_strlen(exp_str) == 0))
	{
		split_words = ft_split(exp_str, ' ');
		word_count = 0;
		while (split_words && split_words[word_count])
			word_count++;
		if (word_count != 1)
		{
			flag_redir_error(exp_str, original_str, word_count);
			shell->last_exit_status = 1;
			return (0);
		}
		redir->del_or_fname = split_words[0];
	}
	else
		redir->del_or_fname = exp_str;
	return (1);
}

static int	expand_and_check_redirs(t_command *cmd, t_shell *shell)
{
	t_redir	*redir;
	int		ex;

	redir = cmd->redirections;
	ex = 0;
	while (redir)
	{
		if (redir->type != REDIR_HEREDOC)
		{
			if (!process_single_redir(redir, shell, &ex))
				return (0);
		}
		redir = redir->next;
	}
	return (1);
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