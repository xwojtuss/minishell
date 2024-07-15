#include "minishell.h"

/*
Prepares the input for execution, prelimenary checks are called from here
*/
void	handle_input(char *input)
{
	char	**array;

	if (input == NULL)
	{
		rl_clear_history();
		exit(EXIT_SUCCESS);
	}
	add_history((const char *)input);
	array = create_array(input);
	if (!array)
		throw_error_exit(input, NULL, NULL);
	if (!check_redirects(array))
		return ;
	execute()
	print_array(array);
	free(array);
}