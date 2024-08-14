#include "minishell.h"

static void	delete_var(t_var **var_list, const char *name)
{
	t_var	*current;
	t_var	*prev;

	current = *var_list;
	prev = NULL;
	while (current != NULL)
	{
		if (ft_strcmp(current->name, (char *)name) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				*var_list = current->next;
			free(current->name);
			free(current->value);
			free(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}

int	ft_unset(int argc, char **argv, t_shell *shell)
// I guess the exit code is always 0 xd
{
	int i;

	i = 1;
	if (argc < 2)
		return (EXIT_SUCCESS);
	while (i < argc)
	{
		if (get_var_value(shell->var, argv[i]))
			delete_var(&shell->var, argv[i]);
		i++;
	}
	return (EXIT_SUCCESS);
}
