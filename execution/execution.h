/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aziane <aziane@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 20:25:10 by aziane            #+#    #+#             */
/*   Updated: 2025/08/11 20:25:10 by aziane           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

# include "../parsing.h"
# include <fcntl.h>
# include <errno.h>

int		set_env(const char *name, const char *value, t_shell *shell);
int		handle_redirections(t_command *cmd);
void	restore_fds(int original_stdin, int original_stdout);
int		wait_for_children(pid_t last_pid);
char	*find_path(char *cmd, char **envp);
int		is_builtin(char *cmd);
void	execute_builtin(t_command *cmd, t_shell *shell);
int		is_parent_builtin(char *cmd);
char	**get_filtered_envp(char **envp);
void	handle_exit(char *id);
pid_t	execute_pipeline_command(t_command *cmd, t_shell *shell,
			int in_fd, int *pipe_fd);
void	parent_handle_pipe_fds(int *in_fd, int *pipe_fd, t_command *cmd);
int		add_new_env_var(char *new_var, t_shell *shell, int size);
char	*create_env_string(const char *name, const char *value);

#endif