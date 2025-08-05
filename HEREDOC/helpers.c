#include "parsing.h"

char    *final_delim(char *delim)
{
    int  i;

    i = 0;    
    while (delim[i])
	{
		if (delim[i] == '$')
			break ;
		i++;
	}
	if (delim[i+1] == '\'' || delim[i+1] == '\"')
	    delim[i] = '\"';
	return (strip_quotes(delim));
}

void	heredoc_sigint_handler(int sig)
{
	(void)sig;
	exit(130);
}

char	*maybe_expand_line(char *line, bool flag, t_shell *shell)
{
	int exp;

	exp = 0;
	if (flag)
		return (expander(line, shell, &exp));
	else
		return (gc_strdup(line));
}