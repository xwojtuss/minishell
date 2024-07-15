#include "minishell.h"

void	init_env(void)
{
	extern char	**environ;
	int			i;

	i = 0;
	while (environ[i])
	{
		printf("environ[%d]: %s\n", i, environ[i]);
		i++;
	}
}