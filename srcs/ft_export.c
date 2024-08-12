#include "minishell.h"

void	print_variables(t_var *var)
{
	t_var	*curr;

	curr = var->next;
	while (curr)
	{
		if (curr->name && curr->value)
			printf("declare -x %s=\"%s\"\n", curr->name, curr->value);
		else if (curr->name)
			printf("declare -x %s\n", curr->name);
		curr = curr->next;
	}
}

t_var	*get_var_struct(t_var *var, char *name)
{
	t_var	*tmp;

	tmp = var;
	while (tmp)
	{
		if (!ft_strcmp(tmp->name, name))
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}

void	add_env_var(t_var **var, char *name, char *value)
{
	t_var	*new;
	t_var	*tmp;

	new = (t_var *)malloc(sizeof(t_var));
	if (!new)
		return ;
	new->name = ft_strdup(name);
	if (!new->name)
		return (free(new));
	new->value = ft_strdup(value);
	if (!new->value)
		return (free(new->name), free(new));
	new->next = NULL;
	tmp = *var;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

/*
Saves or updates the variable in the linked list
if the args is empty it prints all variables, even those without a value
*/
int	ft_export(int argc, char **argv, t_shell *shell)
{
	int		i;
	char	*name;
	char	*value;

	if (argc == 1)
	{
		print_variables(shell->var);
		return (0);
	}
	i = 1;
	while (i < argc)
	{
		name = ft_strndup(argv[i], ft_strchrs_mf(argv[i], "=") - argv[i]);
		if (!name)
			return (EXIT_FAILURE);
		value = ft_strchr(argv[i], '=') + 1;
		if (!value)
			value = "";
		if (get_var_value(shell->var, name))
		{
			free(get_var_struct(shell->var, name)->value);
			get_var_struct(shell->var, name)->value = ft_strdup(value);
			if (!get_var_struct(shell->var, name)->value)
				return (EXIT_FAILURE);
		}
		else
			add_env_var(&shell->var, name, value);
		free(name);
		i++;
	}
	return (EXIT_SUCCESS);
}
