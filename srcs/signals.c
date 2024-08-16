/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkaleta <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 22:06:49 by bkaleta           #+#    #+#             */
/*   Updated: 2024/08/16 22:06:51 by bkaleta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
Sets the global variable g_signum to the signal number,
	calls the handle_input function with NULL as argument,
	which will safely exit the program (FOR NOW !!!!!!!!!)
*/
// void	sig_handler(int num)
// {
// 	if (num == SIGINT || num == SIGQUIT)
// 	{
// 		g_signum = num;
// }

void    sig_handler(int sig)
{
    (void)sig;
	printf("\n");
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
}

void    signal_quit(int sig)
{
    (void)sig;
    rl_on_new_line();
    rl_redisplay();
    write(STDERR_FILENO, "  \b\b", 4);
}

void    init_signals(void)
{
    signal(SIGQUIT, signal_quit);
    signal(SIGINT, sig_handler);
}

void init_child_signals(void)
{
	
    signal(SIGINT, NULL);
    signal(SIGQUIT, NULL);
}

// void    init_signals_duo(void)
// {
//     signal(SIGQUIT, signal_quit_duo);
//     signal(SIGINT, signal_c_duo);
// }

// Funkcja obsługująca EOF (Ctrl+D)
void	handle_eof(void)
{
	write(STDOUT_FILENO, "exit\n", 5);
	exit(EXIT_SUCCESS);
}

void	sig_do_nothing(int num)
{
	(void)num;
}

/* void	sigint_exit(int num)
{
	if (num == SIGINT)
	{
		exit(EXIT_SUCCESS);
	}
} */
