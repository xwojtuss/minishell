#include "minishell.h"

/*
Prints the array of strings, the array must be NULL terminated
*/
void	print_array(char **array)
{
	int i;

	i = 0;
	while (array[i])
	{
		printf("array[%d]: %s\n", i, array[i]);
		i++;
	}
}

void	print_cmd(t_cmd *cmd)
{
	int		i;

	i = 0;
	while (cmd)
	{
		i = 0;
		if (!cmd->argv)
		{
			cmd = cmd->next;
			continue ;
		}
		while (i < cmd->argc && cmd->argv[i])
		{
			printf("cmd->argv[%d]: |%s|\n", i, cmd->argv[i]);
			i++;
		}
		cmd = cmd->next;
	}
}
