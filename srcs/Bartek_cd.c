#include "minishell.h"

void	Bartek_cd(t_cmd *cmd)
{
	const char *path;
	char *home;
	int saved_stderr;

	saved_stderr = dup(STDERR_FILENO);

	dup2(cmd->write_fd, STDERR_FILENO);
	if (cmd->args[1] == NULL || strcmp(cmd->args[1], "~") == 0)
	{
		home = getenv("HOME");
		if (!home)
		{
			write(cmd->write_fd, "cd: HOME not set\n",
				strlen("cd: HOME not set\n"));
			dup2(saved_stderr, STDERR_FILENO);
			close(saved_stderr);
			return ;
		}
		path = home;
	}
	else
		path = cmd->args[1];
	if (chdir(path) != 0)
	{
		write(cmd->write_fd, "cd: ", 4);
		write(cmd->write_fd, path, ft_strlen(path));
		write(cmd->write_fd, ": ", 2);
		perror("");
	}
	dup2(saved_stderr, STDERR_FILENO);
	close(saved_stderr);
}
