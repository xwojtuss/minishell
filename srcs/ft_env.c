#include "minishell.h"

/*
Prints the environment variables
*/
int	ft_env(int argc, char **argv, t_shell *shell)
{
	t_var	*curr;

	(void)argc;
	(void)argv;
	curr = shell->var->next;
	while (curr)
	{
		if (curr->name && curr->value)
			printf("%s=%s\n", curr->name, curr->value);
		curr = curr->next;
	}
	return (EXIT_SUCCESS);
}
