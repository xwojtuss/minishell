#include "minishell.h"

void	throw_error(char *input, char **array, t_cmd *cmd)
{
	rl_clear_history();
	if (input)
		free(input);
	if (array)
		free_array(array);
	if (cmd)
		free_cmd(cmd);
	exit(EXIT_FAILURE);
}