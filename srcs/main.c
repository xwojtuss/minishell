/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkaleta <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 20:33:05 by bkaleta           #+#    #+#             */
/*   Updated: 2024/08/16 20:33:08 by bkaleta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_signum = -42;

void	child_process_in_main(char **envp)
{
	init_child_signals();
	//signal(SIGINT, NULL);
	//signal(SIGQUIT, NULL);
	wait_for_input(envp);
	exit(EXIT_SUCCESS);
}

void	parent_process_in_main(pid_t pid, int status)
{
	//signal(SIGINT, sig_handler);
	//signal(SIGQUIT, signal_quit);
	init_signals();
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
