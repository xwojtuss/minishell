/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkaleta <bkaleta@student.42warsaw.pl>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 12:21:14 by bkaleta           #+#    #+#             */
/*   Updated: 2024/08/18 20:07:41 by bkaleta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	child_process_in_main(char **envp)
{
	init_signals();
	wait_for_input(envp);
	exit(EXIT_SUCCESS);
}

void	parent_process_in_main(pid_t pid, int status)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		exit(WEXITSTATUS(status));
}
/*
Mostly handles signals, creates a child to read stdin
*/

int	main(int argc, char **argv, char **envp)
{
	pid_t				pid;
	int					status;

	(void)argc;
	(void)argv;
	rl_clear_history();
	init_signals();
	while (1)
	{
		status = 0;
		pid = fork();
		if (pid == -1)
			throw_error_exit(NULL, NULL, NULL, NULL);
		if (pid == 0)
			child_process_in_main(envp);
		else
			parent_process_in_main(pid, status);
	}
	return (EXIT_SUCCESS);
}
