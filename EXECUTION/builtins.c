
#include "execution.h"

// A very basic implementation of builtins for testing
// Note: A real implementation of cd, export, unset would modify env list and run in parent
int handle_builtin(t_command *cmd, char **envp)
{
    if (!cmd || !cmd->args || !cmd->args[0])
        return (0);

    if (ft_strncmp(cmd->args[0], "echo", 5) == 0)
    {
        int i = 1;
        while (cmd->args[i])
        {
            ft_putstr_fd(cmd->args[i], 1);
            if (cmd->args[i + 1])
                ft_putstr_fd(" ", 1);
            i++;
        }
        ft_putstr_fd("\n", 1);
        return (1);
    }
    if (ft_strncmp(cmd->args[0], "pwd", 4) == 0)
    {
        char buf[1024];
        if (getcwd(buf, sizeof(buf)))
        {
            ft_putstr_fd(buf, 1);
            ft_putstr_fd("\n", 1);
        }
        return (1);
    }
    if (ft_strncmp(cmd->args[0], "env", 4) == 0)
    {
        int i = 0;
        while (envp && envp[i])
        {
            ft_putstr_fd(envp[i], 1);
            ft_putstr_fd("\n", 1);
            i++;
        }
        return (1);
    }
    if (ft_strncmp(cmd->args[0], "exit", 5) == 0)
    {
        exit(0);
    }
    return (0);
}
