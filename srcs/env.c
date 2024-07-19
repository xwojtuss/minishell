#include "minishell.h"

/*
Prints the environment variables
*/
void	env(t_var *var)
{
	t_var	*curr;

	//start printing without the $? variable
	curr = var->next;
	while (curr)
	{
		if (curr->name && curr->value)
			printf("%s=%s\n", curr->name, curr->value);
		curr = curr->next;
	}
}
