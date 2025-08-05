#include "builtins.h"

int	is_valid_identifier(const char *s)
{
	int	i;

	if (!s || (!ft_isalpha(s[0]) && s[0] != '_'))
		return (0);
	i = 1;
	while (s[i] && s[i] != '=' && s[i] != '+')
	{
		if (!ft_isalnum(s[i]) && s[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static int add_var(char *arg, char *eq, t_shell *shell)
{
    char *name;
    char *value;

    name = ft_substr(arg, 0, eq - arg);
	if (!name)
		return (0);
	value = eq + 1;
	set_env(name, value, shell);
	return (free(name), 1);
}

int add_or_app_var(char *arg, char *eq, t_shell *shell)
{
    char *name;
    char *value;
    char *existing_value;
    char *new_val;

    if (eq && eq > arg && *(eq - 1) == '+')
    {
        name = ft_substr(arg, 0, eq - arg - 1);
        if (!name)
            return (0);
        value = eq + 1;
        existing_value = my_getenv(name, shell->envp);
        if (existing_value)
        {
            new_val = ft_strjoin(existing_value, value);
            if (!new_val)
                return (free(name), 0);
            (set_env(name, new_val, shell), free(new_val));
        }
        else
            set_env(name, value, shell);
        return (free(name), 1);
    }
    else
        return (add_var(arg, eq, shell));
}

int	add_without_value(char *arg, t_shell *shell, int arg_len)
{
	int		i;
	char	**new_envp;

	i = 0;
	while (shell->envp[i])
	{
		if (ft_strncmp(shell->envp[i], arg, arg_len) == 0
			&& (shell->envp[i][arg_len] == '='
				|| shell->envp[i][arg_len] == '\0'))
			return (1);
		i++;
	}
	new_envp = malloc(sizeof(char *) * (i + 2));
	if (!new_envp)
		return (0);
	i = -1;
	while (shell->envp[++i])
		new_envp[i] = shell->envp[i];
	new_envp[i] = ft_strdup(arg);
	if (!new_envp[i])
		return (free(new_envp), 0);
	new_envp[i + 1] = NULL;
	free(shell->envp);
	shell->envp = new_envp;
	return (1);
}