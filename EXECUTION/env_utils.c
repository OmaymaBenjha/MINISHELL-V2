#include "../parsing.h"
#include <stdlib.h>

static int	count_env_vars(char **envp)
{
	int	i;

	i = 0;
	if (!envp)
		return (0);
	while (envp[i])
		i++;
	return (i);
}

void	free_env(char **envp)
{
	int	i;

	if (!envp)
		return ;
	i = 0;
	while (envp[i])
	{
		free(envp[i]);
		i++;
	}
	free(envp);
}

char	**dupenv(char **envp)
{
	int		i;
	int		count;
	char	**new_envp;
	int 	a;

	count = count_env_vars(envp);
	new_envp = (char **)malloc(sizeof(char *) * (count + 1));
	if (!new_envp)
		return (NULL);
	i = 0;
	a = 0;
	while (i < count)
	{
		if (ft_strncmp(envp[i], "OLDPWD", 6) == 0)
			i++;
		new_envp[a] = ft_strdup(envp[i]);
		if (!new_envp[a])
		{
			while (--a >= 0)
				free(new_envp[a]);
			free(new_envp);
			return (NULL);
		}
		i++;
		a++;
	}
	new_envp[i] = NULL;
	return (new_envp);
}

char	*my_getenv(const char *name, char **env)
{
	int		i;
	size_t	name_len;

	if (!name || !env)
		return (NULL);
	name_len = ft_strlen(name);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], name, name_len) == 0)
		{
			if (env[i][name_len] == '=')
				return (env[i] + name_len + 1);
		}
		i++;
	}
	return (NULL);
}

int	set_env(const char *name, const char *value, t_shell *shell)
{
	int		i;
	size_t	name_len;
	char	*new_var;
	char	*temp;
	char	**new_envp;

	if (value != (char const *)NULL)
	{
		name_len = ft_strlen(name);
		temp = ft_strjoin(name, "=");
		new_var = ft_strjoin(temp, value);	
		free(temp);
	}
	else 
		new_var = ft_strdup((char *)name);
	if (!new_var)
		return (1);
	i = 0;
	while (shell->envp[i])
	{
		if (ft_strncmp(shell->envp[i], name, name_len) == 0)
		{
			free(shell->envp[i]);
			shell->envp[i] = new_var;
			return (0);
		}
		i++;
	}
	new_envp = malloc(sizeof(char *) * (i + 2));
	if (!new_envp)
	{
		free(new_var);
		return (1);
	}
	i = -1;
	while (shell->envp[++i])
		new_envp[i] = shell->envp[i];
	new_envp[i] = new_var;
	new_envp[i + 1] = NULL;
	free(shell->envp);
	shell->envp = new_envp;
	return (0);
}

char	**get_filtered_envp(char **envp)
{
	int		i;
	int		j;
	int		count;
	char	**filtered_envp;

	count = 0;
	i = 0;
	while (envp[i])
	{
		if (ft_strchr(envp[i], '='))
			count++;
		i++;
	}
	filtered_envp = malloc(sizeof(char *) * (count + 1));
	if (!filtered_envp)
		return (NULL);
	i = 0;
	j = 0;
	while (envp[i])
	{
		if (ft_strchr(envp[i], '='))
			filtered_envp[j++] = envp[i];
		i++;
	}
	filtered_envp[j] = NULL;
	return (filtered_envp);
}
