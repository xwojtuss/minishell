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

/*
Saves or updates the variable in the linked list
if the args is empty it prints all variables, even those without a value
*/
void	export(t_var *var, t_cmd *cmd)
{
	char	buffer[BUFFER_SIZE + 1];
	int		bytes;
	char	*args;

	if (!cmd->args || !cmd->args[1])//TODO: it works with multiple arguments, it has to use split!!!
	{
		args = ft_strdup("");
		if (!args)
			throw_error_exit(NULL, NULL, cmd, var);
		while (1)
		{
			bytes = read(cmd->read_fd, buffer, BUFFER_SIZE);
			if (bytes == 0)
				break ;
			if (bytes == -1)
			{
				free(args);
				throw_error_exit(NULL, NULL, cmd, var);
			}
			buffer[bytes] = '\0';
			args = ft_strjoin(buffer, args);
			if (!args)
				throw_error_exit(NULL, NULL, cmd, var);
		}
	}
	else
	{
		//this is wrong, it works with multiple arguments, it has to use all args
		args = ft_strdup(cmd->args[0]);//or maybe strdup, depending on the implementation (it might be wise to check if args[0] is NULL -> free(args))
		//execute with the args
	}
	if (is_empty(args))
	{
		free(args);//!!! check if this is correct
		return ;
	}
	t_var	*new;
	new = (t_var *)malloc(sizeof(t_var));
	if (!new)
	{
		free(args);//!!! check if this is correct
		throw_error_exit(NULL, NULL, cmd);
	}
	new->next = NULL;
	new->name = get_name(args);
	if (!new->name)
	{
		free(args);//!!! check if this is correct
		free(new);
		throw_error_exit(NULL, NULL, cmd);
	}
	new->value = get_value(args);
	if (!new->value)
	{
		free(args);//!!! check if this is correct
		free(new->name);
		free(new);
		throw_error_exit(NULL, NULL, cmd);
	}
	if (!is_correct(new->name) || !is_correct(new->value))
	{
		free(args);//!!! check if this is correct
		free(new->name);
		free(new->value);
		free(new);
		throw_error_exit(NULL, NULL, cmd);
	}
	get_last_var(var)->next = new;
}
