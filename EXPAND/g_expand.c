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

static int	process_single_redir(t_redir *redir, t_shell *shell, int *ex)
{
	char	*original_str;
	char	*exp_str;
	char	**split_words;
	int		word_count;

	original_str = redir->del_or_fname;
	exp_str = expander(original_str, shell, ex);
	if (((*ex) && !ft_strchr(original_str, '\"')
			&& !ft_strchr(original_str, '\'')) || (ft_strlen(exp_str) == 0)
		|| ft_strchr(exp_str, '$'))
	{
		split_words = ft_split(exp_str, ' ');
		word_count = 0;
		while (split_words && split_words[word_count])
			word_count++;
		flag_redir_error(exp_str, original_str, word_count);
		redir->del_or_fname = split_words[0];
	}
	else
		redir->del_or_fname = exp_str;
	return (1);
}

static void	expand_and_split_arg(char *arg, t_shell *shell, t_arg_list **head)
{
	int		has_expanded;
	char	*exp_str;
	char	**split_words;
	int		j;

	has_expanded = 0;
	exp_str = expander(arg, shell, &has_expanded);
	if ((has_expanded && !ft_strchr(arg, '\"') && !ft_strchr(arg, '\''))
		|| (ft_strlen(exp_str) == 0))
	{
		split_words = ft_split(exp_str, ' ');
		j = 0;
		while (split_words && split_words[j])
			add_arg_to_list(head, split_words[j++]);
	}
	else
		add_arg_to_list(head, exp_str);
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
