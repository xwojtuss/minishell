/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_processes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkaleta <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 22:06:57 by bkaleta           #+#    #+#             */
/*   Updated: 2024/08/16 22:07:08 by bkaleta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	child_routine(t_shell *shell, t_cmd *cmd, int pipe_fd[2],
		int prev_read_fd)
{
	if (!open_redirs(cmd))
		safely_exit(0, shell, NULL, NULL);
	duplicate_read_fd(cmd, prev_read_fd);
	duplicate_write_fd(cmd, pipe_fd);
	if (is_builtin(cmd->argv[0]))
		exit(execute_builtin(cmd, shell, true));
	else
		exit(execute_external(cmd, shell));
}

void	parent_routine(t_cmd *cmd, int pipe_fd[2], int *prev_read_fd)
{
	if (*prev_read_fd != NOT_SET)
		close(*prev_read_fd);
	if (cmd->next)
	{
		if (pipe_fd[1] != NOT_SET)
			close(pipe_fd[1]);
		*prev_read_fd = pipe_fd[0];
	}
	else
		*prev_read_fd = NOT_SET;
}

pid_t	create_child(t_shell *shell, t_cmd *cmd)
{
	int			pipe_fd[2];
	static int	prev_read_fd = NOT_SET;
	pid_t		pid;

	pipe_fd[0] = NOT_SET;
	pipe_fd[1] = NOT_SET;
	if (cmd->next)
		if (pipe(pipe_fd) == -1)
			return (-1);
	pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0)
		child_routine(shell, cmd, pipe_fd, prev_read_fd);
	else
		parent_routine(cmd, pipe_fd, &prev_read_fd);
	return (pid);
}

void	wait_for_processes(t_shell *shell, pid_t last_pid)
{
	int	status;
	int	i;

	status = 0;
	i = count_cmds(shell->cmd);
	while (i > 0)
	{
		if (waitpid(0, &status, 0) == last_pid)
			set_last_exit_code(shell->var, WEXITSTATUS(status));
		i--;
	}
}
