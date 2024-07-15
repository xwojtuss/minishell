#include "minishell.h"

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

void	free_cmd(t_cmd *cmd)
{
	t_cmd *tmp;

	while (cmd)
	{
		if (!cmd->next)
			return ;
		tmp = cmd->next;
		free(cmd->cmd);
		free_array(cmd->args);
		free(cmd);
		cmd = tmp;
	}
}