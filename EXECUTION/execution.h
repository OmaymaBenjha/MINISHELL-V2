
#ifndef EXECUTION_H
# define EXECUTION_H

# include "parsing.h"
# include <unistd.h>
# include <sys/wait.h>
# include <fcntl.h>

// Main executor function
void    executor(t_command *commands, char **envp);

#endif
