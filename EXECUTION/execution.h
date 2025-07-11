#ifndef EXECUTION_H
# define EXECUTION_H

# include "../parsing.h"
# include <fcntl.h>
# include <sys/wait.h>
# include <string.h>
# include <errno.h>

// Note: executor and env functions are now in parsing.h
// to avoid circular dependencies.

int		handle_redirections(t_command *cmd);
void	restore_fds(int original_stdin, int original_stdout);
int		wait_for_children(pid_t last_pid);

char	*find_path(char *cmd, char **envp);

int		is_builtin(char *cmd);
void	execute_builtin(t_command *cmd, t_shell *shell);
int		is_parent_builtin(char *cmd);

#endif
