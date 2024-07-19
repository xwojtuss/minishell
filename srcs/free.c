#include "minishell.h"

/*
Frees the array of strings, the array must be NULL terminated
*/
void	free_array(char **array)
{
	int i;

	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

/*
Free the var linked list
*/
void	free_var(t_var *var)
{
	t_var *tmp;

	while (var)
	{
		tmp = var->next;
		if (var->name)
			free(var->name);
		if (var->value)
			free(var->value);
		free(var);
		var = tmp;
	}
}

/*
Free the cmd linked list
*/
void	free_cmd(t_cmd *cmd)
{
	t_cmd *tmp;

	while (cmd)
	{
		tmp = cmd->next;
		if (cmd->cmd)
			free(cmd->cmd);
		if (cmd->args)
			free_array(cmd->args);
		free(cmd);
		cmd = tmp;
	}
}