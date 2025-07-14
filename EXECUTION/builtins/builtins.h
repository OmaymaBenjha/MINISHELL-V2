#ifndef BUILTINS_H
# define BUILTINS_H

# include "../execution.h"

int		ft_cd(char **args, t_shell *shell);
int		ft_echo(char **args);
int		ft_env(t_shell *shell);
int		ft_exit(char **args, t_shell *shell);
int		ft_export(char **args, t_shell *shell);
int		ft_pwd(t_shell *shell);
int		ft_unset(char **args, t_shell *shell);

#endif