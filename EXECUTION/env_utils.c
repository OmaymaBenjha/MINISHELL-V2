#include "../parsing.h"

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

char	**dupenv(char **envp)
{
	int		i;
	int		count;
	char	**new_envp;

	count = count_env_vars(envp);
	new_envp = (char **)gc_mall(sizeof(char *) * (count + 1));
	i = 0;
	while (i < count)
	{
		new_envp[i] = gc_strdup(envp[i]);
		i++;
	}
	new_envp[i] = NULL;
	return (new_envp);
}
