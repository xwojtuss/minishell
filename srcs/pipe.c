/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkornato <wkornato@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 13:51:07 by wkornato          #+#    #+#             */
/*   Updated: 2024/08/15 00:23:10 by wkornato         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
--TESTING--
a test of a one-pipe functionality
*/
int	pipe_exec(char **cmd1, char **cmd2)
{
	int fd[2];
	pid_t pid;
	pid_t pid2;

	if (pipe(fd) == -1)
		return (EXIT_FAILURE);
	pid = fork();
	if (pid == -1)
		return (EXIT_FAILURE);
	if (pid == 0)
	{
		dup2(fd[1], STDOUT_FILENO);
		close(fd[0]);
		close(fd[1]);
		execve(cmd1[0], cmd1, NULL);
		exit(EXIT_FAILURE);
	}
	pid2 = fork();
	if (pid2 == -1)
		return (EXIT_FAILURE);
	if (pid2 == 0)
	{
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		close(fd[1]);
		execve(cmd2[0], cmd2, NULL);
		exit(EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
