/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_finder.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aziane <aziane@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 20:58:56 by aziane            #+#    #+#             */
/*   Updated: 2025/08/11 20:58:56 by aziane           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

static int	is_directory(const char *path)
{
	int		fd;
	char	buffer;

	fd = open(path, O_RDONLY);
	if (fd == -1)
		return (0);
	if (read(fd, &buffer, 0) == -1 && errno == EISDIR)
	{
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

static char	*get_path_from_env(char **envp)
{
	int	i;

	i = 0;
	while (envp && envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
			return (envp[i] + 5);
		i++;
	}
	return (NULL);
}

static char	*check_paths(char *cmd, char **paths)
{
	int		i;
	char	*path_part;
	char	*full_path;

	i = 0;
	while (paths[i])
	{
		path_part = gc_strjoin(paths[i], "/");
		full_path = gc_strjoin(path_part, cmd);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		i++;
	}
	return (NULL);
}

static char	*check_absolute_path(char *cmd)
{
	if (is_directory(cmd))
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": is a directory\n", 2);
		exit(126);
	}
	if (access(cmd, X_OK) < 0)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(cmd);
		if (errno == ENOTDIR || errno == EACCES)
			exit(126);
		else
			exit(127);
	}
	return (gc_strdup(cmd));
}

char	*find_path(char *cmd, char **envp)
{
	char	**paths;
	char	*path_env;
	char	*executable_path;

	if (!cmd || *cmd == '\0')
		(ft_putstr_fd("minishell: : command not found\n", 2), exit(127));
	if (ft_strchr(cmd, '/'))
		return (check_absolute_path(cmd));
	path_env = get_path_from_env(envp);
	if (path_env)
	{
		paths = ft_split(path_env, ':');
		executable_path = check_paths(cmd, paths);
		if (executable_path)
			return (executable_path);
	}
	if (access(cmd, X_OK) == 0)
		return (gc_strdup(cmd));
	if (path_env)
	{
		(ft_putstr_fd("minishell: ", 2), ft_putstr_fd(cmd, 2));
		(ft_putstr_fd(": command not found\n", 2), exit(127));
	}
	(ft_putstr_fd("minishell: ", 2), perror(cmd), exit(127));
}
