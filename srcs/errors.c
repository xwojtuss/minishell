#include "minishell.h"

/*
Clears the history, frees the input, the array and the cmd if they are not null, exits with EXIT_FAILURE
*/
void	throw_error_exit(char *input, char **array, t_cmd *cmd, t_var *var)
{
	rl_clear_history();
	if (input)
		free(input);
	if (array)
		free_array(array);
	if (cmd)
		free_cmd(cmd);
	if (var)
		free_var(var);
	exit(EXIT_FAILURE);
}
