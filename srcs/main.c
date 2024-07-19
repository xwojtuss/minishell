#include "minishell.h"

int	g_signum = -42;

/*
Mostly handles signals, creates a child to read stdin
*/
int	main(int argc, char **argv, char **envp)
{
	pid_t	pid;
	int status;

	(void)argc;
	(void)argv;
	while (1)
	{
		status = 0;
		pid = fork();
		if (pid == -1)
			throw_error_exit(NULL, NULL, NULL, NULL);
		if (pid == 0)
		{
			signal(SIGINT, NULL);
			signal(SIGQUIT, NULL);
			wait_for_input(envp);
			exit(EXIT_SUCCESS);
		}
		else
		{
			signal(SIGINT, sig_handler);
			signal(SIGQUIT, sig_handler);
			waitpid(pid, &status, 0);
			if (WIFEXITED(status))
				exit(WEXITSTATUS(status));
		}
	}
	return (EXIT_SUCCESS);
}
