#include "minishell.h"

void	Bartek_handle_error(const char *message_to_print)
{
	if (message_to_print)
		perror(message_to_print);
	exit(EXIT_FAILURE);
}