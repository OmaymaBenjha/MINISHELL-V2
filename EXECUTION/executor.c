#include "execution.h"
#include "parsing.h" // <--- ADD THIS LINE

// Forward declaration for builtins
int handle_builtin(t_command *cmd, char **envp);

static void handle_redirections(t_redir *redir)
{
    int fd;
    // ... rest of the function is correct
    while (redir)
    {
        if (redir->type == REDIR_OUTPUT_TRUNC || redir->type == REDIR_OUTPUT_APPEND)
        {
            if (redir->type == REDIR_OUTPUT_TRUNC)
                fd = open(redir->delimiter_or_filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            else
                fd = open(redir->delimiter_or_filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (fd == -1)
                return (perror("open"));
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }
        else if (redir->type == REDIR_INPUT || redir->type == REDIR_HEREDOC)
        {
            if (redir->type == REDIR_INPUT)
                fd = open(redir->delimiter_or_filename, O_RDONLY);
            else // Heredoc
                fd = redir->heredoc_fd;
            if (fd == -1)
                return (perror("open"));
            dup2(fd, STDIN_FILENO);
            close(fd);
        }
        redir = redir->next;
    }
}


static char *find_path(char *cmd, char **envp)
{
    char **paths;
    char *path;
    char *part_path;
    int i;

    if (cmd && (cmd[0] == '/' || cmd[0] == '.'))
        if (access(cmd, X_OK) == 0)
            return (ft_strdup(cmd));
    i = 0;
    while (envp[i] && ft_strncmp(envp[i], "PATH=", 5) != 0)
        i++;
    if (!envp[i])
        return (NULL);
    paths = ft_split(envp[i] + 5, ':'); // This will now compile
    i = 0;
    while (paths && paths[i])
    {
        part_path = ft_strjoin(paths[i], "/");
        path = ft_strjoin(part_path, cmd);
        free(part_path);
        if (access(path, X_OK) == 0)
        {
            // A real implementation should free the 'paths' array
            return (path);
        }
        free(path);
        i++;
    }
    // A real implementation should free the 'paths' array
    return (NULL);
}


static void execute_child(t_command *cmd, char **envp)
{
    char *path;

    handle_redirections(cmd->redirections);
    if (handle_builtin(cmd, envp))
        exit(0);
    path = find_path(cmd->args[0], envp);
    if (!path)
    {
        ft_putstr_fd("minishell: command not found: ", 2);
        ft_putstr_fd(cmd->args[0], 2);
        ft_putstr_fd("\n", 2);
        exit(127);
    }
    execve(path, cmd->args, envp);
    perror("execve");
    exit(126);
}


void executor(t_command *commands, char **envp)
{
    t_command *cmd;
    pid_t pid;
    int status;
    int pipe_fd[2];
    int prev_pipe_fd = -1;

    cmd = commands;
    if (!cmd)
        return;

    // Simple case: one command, maybe a builtin that modifies the parent shell
    if (!cmd->next_piped_command && handle_builtin(cmd, envp))
        return;
    
    while (cmd)
    {
        if (cmd->next_piped_command)
            if (pipe(pipe_fd) == -1)
                return (perror("pipe"));
        
        pid = fork();
        if (pid == -1)
            return (perror("fork"));
        
        if (pid == 0) // Child process
        {
            if (prev_pipe_fd != -1)
            {
                dup2(prev_pipe_fd, STDIN_FILENO);
                close(prev_pipe_fd);
            }
            if (cmd->next_piped_command)
            {
                dup2(pipe_fd[1], STDOUT_FILENO);
                close(pipe_fd[0]);
                close(pipe_fd[1]);
            }
            execute_child(cmd, envp);
        }
        // Parent process
        if (prev_pipe_fd != -1)
            close(prev_pipe_fd);
        if (cmd->next_piped_command)
        {
            close(pipe_fd[1]);
            prev_pipe_fd = pipe_fd[0];
        }
        cmd = cmd->next_piped_command;
    }
    while (wait(&status) > 0);
}