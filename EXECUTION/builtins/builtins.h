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
int     add_or_app_var(char *arg, char *eq, t_shell *shell);
int     add_without_value(char *arg, t_shell *shell, int arg_len);
int     is_valid_identifier(const char *s);


#endif