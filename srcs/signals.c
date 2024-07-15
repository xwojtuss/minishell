#include "minishell.h"

/*
Sets the global variable g_signum to the signal number, calls the handle_input function with NULL as argument, which will safely exit the program (FOR NOW !!!!!!!!!)
*/
void sig_handler(int num)
{
	char	*cwd;
	char	*prompt;

	g_signum = num;
	if (num == SIGINT)
	{
		write(1, "\n", 1);
		// It's either just spaces or path + spaces
		cwd = getcwd(NULL, 0);
		if (!cwd)
			throw_error_exit(NULL, NULL, NULL);
		prompt = ft_strjoin(cwd, "$ ");
		free(cwd);
		if (!prompt)
			throw_error_exit(NULL, NULL, NULL);
		printf("%s", prompt);
		// I don't know what else to do but this doesn't work fully
	}
	else
		handle_input(NULL);
}