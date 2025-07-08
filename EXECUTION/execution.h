#ifndef EXECUTION_H
# define EXECUTION_H

# include "../parsing.h"
# include "status.h"
# include <fcntl.h>
# include <sys/wait.h>
# include <string.h>
# include <errno.h>

void	executor(t_command *commands, char ***envp);
char	**dupenv(char **envp);
void	free_env(char **envp);

int		handle_redirections(t_command *cmd);
void	restore_fds(int original_stdin, int original_stdout);
int		wait_for_children(pid_t last_pid);

char	*find_path(char *cmd, char **envp);

int		is_builtin(char *cmd);
void	execute_builtin(t_command *cmd, char ***envp_ptr);
int		is_parent_builtin(char *cmd);

#endif
