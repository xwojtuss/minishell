#include "minishell.h"

int	g_signum = 0;

void	handle_input(const char *input)
{
	if (input == NULL)
	{
		rl_clear_history();
		exit(EXIT_SUCCESS);
	}
	add_history(input);
	printf("You entered: %s\n", input);
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

