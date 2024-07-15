#include "minishell.h"

/*
Sets the global variable g_signum to the signal number, calls the handle_input function with NULL as argument, which will safely exit the program (FOR NOW !!!!!!!!!)
*/
void sig_handler(int num)
{
	g_signum = num;
	handle_input(NULL);
}