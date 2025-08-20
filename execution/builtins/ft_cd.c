/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aziane <aziane@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 17:16:44 by aziane            #+#    #+#             */
/*   Updated: 2025/08/16 17:16:44 by aziane           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include <unistd.h>
#include <stdio.h>

static void	set_pwd_vars(t_shell *shell, char *oldpwd)
{
	if (oldpwd && oldpwd[0] != '\0')
		set_env("OLDPWD", oldpwd, shell);
	set_env("PWD", shell->cwd, shell);
}

static char	*get_cd_path(char **args, t_shell *shell)
{
	char	*path;
	char	*tmp;

	path = args[1];
	if (!path)
	{
		path = my_getenv("HOME", shell->envp);
		if (!path)
			ft_putstr_fd("minishell: cd: HOME not set\n", 2);
	}
	else if (ft_strcmp(path, "-") == 0)
	{
		tmp = my_getenv("OLDPWD", shell->envp);
		if (!tmp)
		{
			ft_putstr_fd("minishell: cd: OLDPWD not set\n", 2);
			return (NULL);
		}
		path = tmp;
		ft_putstr_fd(path, 1);
		ft_putstr_fd("\n", 1);
	}
	return (path);
}

static void	update_cwd(t_shell *shell, char *path, char *oldpwd)
{
	char	*tmp;
	char	*join;

	if (getcwd(shell->cwd, PATH_MAX) == NULL)
	{
		if (path[0] == '/')
			tmp = ft_strdup(path);
		else
		{
			join = ft_strjoin(oldpwd, "/");
			tmp = ft_strjoin(join, path);
			free(join);
		}
		if (tmp)
		{
			ft_strlcpy(shell->cwd, tmp, PATH_MAX);
			free(tmp);
		}
		perror("minishell: cd: error retrieving current directory");
	}
}

int	ft_cd(char **args, t_shell *shell)
{
	char	*path;
	char	*oldpwd;

	chdir(shell->cwd);
	if (args[1] && args[2])
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		return (1);
	}
	path = get_cd_path(args, shell);
	if (!path)
		return (1);
	oldpwd = ft_strdup(shell->cwd);
	if (chdir(path) != 0)
	{
		free(oldpwd);
		ft_putstr_fd("minishell: cd: ", 2);
		perror(path);
		return (1);
	}
	update_cwd(shell, path, oldpwd);
	set_pwd_vars(shell, oldpwd);
	free(oldpwd);
	return (0);
}
