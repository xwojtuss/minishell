#include "minishell.h"

/*
Sets the global variable g_signum to the signal number,
	calls the handle_input function with NULL as argument,
	which will safely exit the program (FOR NOW !!!!!!!!!)
*/
void	sig_handler(int num)
{
	if (num == SIGINT || num == SIGQUIT)
	{
		g_signum = num;
	}
}

void	sig_do_nothing(int num)
{
	(void)num;
}

/* void	sigint_exit(int num)
{
	if (num == SIGINT)
	{
		exit(EXIT_SUCCESS);
	}
} */
