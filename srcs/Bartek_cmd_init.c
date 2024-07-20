#include "minishell.h"

t_cmd	*Bartek_init_cmd(char **array)
{
	t_cmd	*cmd;

	cmd = (t_cmd *)malloc(sizeof(t_cmd));
	if (!cmd)
		return NULL;

	cmd->args = array;
	cmd->read_fd = STDIN_FILENO;  // Default to standard input
	cmd->write_fd = STDOUT_FILENO; // Default to standard output
	cmd->next = NULL;

	return (cmd);
}