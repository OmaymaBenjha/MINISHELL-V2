#include "status.h"

static int	g_shell_exit_status = 0;

void	set_exit_status(int status)
{
	g_shell_exit_status = status;
}

int	get_exit_status(void)
{
	return (g_shell_exit_status);
}
