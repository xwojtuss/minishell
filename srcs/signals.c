/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkornato <wkornato@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 12:21:05 by bkaleta           #+#    #+#             */
/*   Updated: 2024/08/25 12:47:02 by wkornato         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sig_handler(int sig)
{
	(void)sig;
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	signal_quit(int sig)
{
	(void)sig;
	rl_on_new_line();
	rl_redisplay();
	write(STDERR_FILENO, "  \b\b", 4);
}

void	init_signals(void)
{
	signal(SIGQUIT, signal_quit);
	signal(SIGINT, sig_handler);
}

void	sig_kill(int signum)
{
	signal(signum, SIG_IGN);
	kill(0, signum);
	if (signum == SIGQUIT)
		printf("Quit");
	printf("\n");
	rl_on_new_line();
}

void	handle_eof(t_shell *shell)
{
	free_var(shell->var);
	write(STDOUT_FILENO, "exit\n", 5);
	exit(EXIT_SUCCESS);
}
