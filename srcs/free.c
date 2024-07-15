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
Free the cmd linked list
*/
void	free_cmd(t_cmd *cmd)
{
	t_cmd *tmp;

	while (cmd)
	{
		tmp = cmd->next;
		free(cmd->cmd);
		free_array(cmd->args);
		free(cmd);
		cmd = tmp;
	}
}