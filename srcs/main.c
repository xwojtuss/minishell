#include "minishell.h"

int	g_signum = 0;

void	handle_input(char *input)
{
	// t_cmd	*cmd;
	char	**array;

	if (input == NULL)
	{
		rl_clear_history();
		exit(EXIT_SUCCESS);
	}
	add_history((const char *)input);
	array = create_array(input);
	if (!array)
		throw_error(input, NULL, NULL);
	print_array(array);
	free(array);
}

int	main(void)
{
	char	*input;

	signal(SIGINT, sig_handler);
	signal(SIGQUIT, sig_handler);
	rl_clear_history();
	while (1)
	{
		input = readline("minishell> ");
		handle_input(input);
		free(input);
	}
	return (EXIT_SUCCESS);
}

