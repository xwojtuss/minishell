#include "minishell.h"

/*
Frees and closes the files
*/
void	free_files(t_files *files)
{
	t_files *tmp;

	while (files)
	{
		tmp = files->next;
		if (files->path)
			free(files->path);
		if (files->fd != -1)
			close(files->fd);
		free(files);
		files = tmp;
	}
}

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
		if (cmd->argv)
			free_array(cmd->argv);
		free(cmd);
		cmd = tmp;
	}
}