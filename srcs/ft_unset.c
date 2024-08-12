#include "minishell.h"

static void delete_var(t_var **var_list, const char *name)
{
    t_var *current = *var_list;
    t_var *prev = NULL;

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

int	ft_unset(int argc, char **argv, t_shell *shell)//I guess the exit code is always 0 xd
{
	int i;

	if (!shell || argc < 2)
		return (EXIT_FAILURE);
	i = 1;
	while (i < argc)
	{
		if (get_var_value(shell->var, argv[i]))
			delete_var(&shell->var, argv[i]);
		i++;
	}
	return (EXIT_SUCCESS);
}
