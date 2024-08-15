#include "minishell.h"

int	get_prev_write_fd(t_cmd *cmd, t_cmd *curr)
{
	while (cmd->next != curr)
		cmd = cmd->next;
	if (!cmd->next)
		return (STDIN_FILENO);
	return (cmd->write_fd);
}

int	get_next_read_fd(t_cmd *curr)
{
	if (curr->next)
		return (curr->next->read_fd);
	return (STDOUT_FILENO);
}

void	duplicate_fd(int fd, int std_fd)
{
	if (fd != NOT_SET && !isatty(fd))
	{
		dup2(fd, std_fd);
		if (fd > 0)
			close(fd);
	}
}

void	duplicate_read_fd(t_cmd *cmd, int prev_read_fd)
{
	if (cmd->read_fd > 0 && !isatty(cmd->read_fd))
	{
		dup2(cmd->read_fd, STDIN_FILENO);
		if (cmd->read_fd > 0)
			close(cmd->read_fd);
	}
	else if (prev_read_fd != NOT_SET)
	{
		dup2(prev_read_fd, STDIN_FILENO);
		if (cmd->read_fd > 0)
			close(prev_read_fd);
	}
}

void	duplicate_write_fd(t_cmd *cmd, int pipe_fd[2])
{
	if (cmd->write_fd != NOT_SET && !isatty(cmd->write_fd))
	{
		dup2(cmd->write_fd, STDOUT_FILENO);
		if (cmd->write_fd > 0)
			close(cmd->write_fd);
	}
	else if (cmd->next && pipe_fd[1] != NOT_SET)
	{
		dup2(pipe_fd[1], STDOUT_FILENO);
		if (pipe_fd[1] > 0)
			close(pipe_fd[1]);
		if (pipe_fd[0] > 0 && !isatty(pipe_fd[0]))
			close(pipe_fd[0]);
	}
}
