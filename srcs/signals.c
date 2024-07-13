#include "minishell.h"

void sig_handler(int num)
{
	g_signum = num;
	handle_input(NULL);
}