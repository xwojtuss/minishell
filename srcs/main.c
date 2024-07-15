#include "minishell.h"

int	g_signum = 0;

/*
Mostly handles signals
*/
int	main(void)
{
	signal(SIGINT, sig_handler);
	signal(SIGQUIT, sig_handler);
	rl_clear_history();
	wait_for_input();
	return (EXIT_SUCCESS);
}

