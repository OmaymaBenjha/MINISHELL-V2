#ifndef BUILTINS_H
# define BUILTINS_H

# include "../execution.h"

int		ft_cd(char **args, char **envp);
int		ft_echo(char **args);
int		ft_env(char **envp);
int		ft_exit(char **args);
int		ft_export(char **args, char ***envp_ptr);
int		ft_pwd(void);
int		ft_unset(char **args, char ***envp_ptr);

#endif
