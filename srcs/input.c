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
	if (!*input)
		return ;
	add_history((const char *)input);
	array = create_array(input);
	if (!array)
		throw_error_exit(input, NULL, NULL);
	// init_env();
	if (!check_redirects(array))
		return ;
	// execute()
	print_array(array);
	free(array);
}

void	wait_for_input(void)
{
	char	*input;
	char	*cwd;
	char	*prompt;

	while (1)
	{
		cwd = getcwd(NULL, 0);
		if (!cwd)
			throw_error_exit(NULL, NULL, NULL);
		prompt = ft_strjoin(cwd, "$ ");
		free(cwd);
		if (!prompt)
			throw_error_exit(NULL, NULL, NULL);
		input = readline(prompt);
		handle_input(input);
		free(input);
	}
}