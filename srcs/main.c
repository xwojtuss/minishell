#include "minishell.h"

int	g_signum = 0;

/*
Mostly handles signals
*/
int	main(void)
{
	char	*input;
	char	*cwd;
	char	*prompt;

	signal(SIGINT, sig_handler);
	signal(SIGQUIT, sig_handler);
	rl_clear_history();
	getenv("PWD");
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
	return (EXIT_SUCCESS);
}

