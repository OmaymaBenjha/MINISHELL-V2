#include "builtins.h"

static int is_numeric_char(char c)
{
    return (c >= '0' && c <= '9');
}

static int is_valid_exit_arg(char *n)
{
    int i = 0;

    if (!n)
        return (0);
    if (n[i] == '+' || n[i] == '-')
        i++;
    if (n[i] == '\0') 
        return (0);
    while (n[i])
    {
        if (!is_numeric_char(n[i]))
            return (0);
        i++;
    }
    return (1);
}

int ft_exit(char **args, t_shell *shell)
{
    long long status;

    ft_putstr_fd("exit\n", 1);
    if (args[1] && args[2])
    {
        ft_putstr_fd("minishell: exit: too many arguments\n", 2);
        return (1);
    }
    if (args[1])
    {
        if (!is_valid_exit_arg(args[1]))
        {
            ft_putstr_fd("minishell: exit: ", 2);
            ft_putstr_fd(args[1], 2);
            ft_putstr_fd(": numeric argument required\n", 2);
            exit(255); 
        }
        status = ft_atoi(args[1]);
        exit((unsigned char)status); 
    }
    exit(shell->last_exit_status);
    return (0);
}
