#include "minishell.h"

void	Bartek_handle_error_pwd(const char *message_to_print)
{
	if (message_to_print)
		perror(message_to_print);
	exit(EXIT_FAILURE);
}
void	Bartek_pwd(t_cmd *cmd)
{
	char	*curr_work_dir;

	curr_work_dir = getcwd(NULL, 0);
	if (!curr_work_dir)
		Bartek_handle_error_pwd("get current working directory");
	
	write(cmd->write_fd, curr_work_dir, ft_strlen(curr_work_dir));
	write(cmd->write_fd, "\n", 1);

	free(curr_work_dir);
}