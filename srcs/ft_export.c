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
	if (!new->value && value)
		return (free(new->name), free(new));
	new->next = NULL;
	tmp = *var;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

bool	is_a_number(char *s)
{
	int	i;

	if (!s)
		return (false);
	i = 0;
	while (s[i])
	{
		if (!ft_isdigit(s[i]))
			return (false);
		i++;
	}
	return (true);
}

static void	update_var(t_shell *shell, char *name, char *line)
{
	char	*value;

	if (ft_strchr(line, '='))
		value = ft_strchr(line, '=') + 1;
	else
		value = NULL;
	if (get_var_value(shell->var, name))
	{
		free(get_var_struct(shell->var, name)->value);
		get_var_struct(shell->var, name)->value = ft_strdup(value);
	}
	else
		add_env_var(&shell->var, name, value);
}

/*
Saves or updates the variable in the linked list
if the args is empty it prints all variables, even those without a value
*/
int	ft_export(int argc, char **argv, t_shell *shell)
{
	int		i;
	char	*name;

	if (argc == 1)
		return (print_variables(shell->var), 0);
	i = 1;
	while (i < argc)
	{
		name = ft_strndup(argv[i], ft_strchrs_mf(argv[i], "=") - argv[i]);
		if (!name)
			return (EXIT_FAILURE);
		if (ft_strchr(name, '-') || ft_strchr(name, '+') || ft_strchr(argv[i],
				'=') == argv[i] || is_a_number(name))
		{
			ft_putstr_fd("minishell: export: '", STDERR_FILENO);
			ft_putstr_fd(name, STDERR_FILENO);
			ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
			return (free(name), EXIT_FAILURE);
		}
		update_var(shell, name, argv[i]);
		free(name);
		i++;
	}
	return (EXIT_SUCCESS);
}
