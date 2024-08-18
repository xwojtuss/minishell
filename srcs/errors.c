/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkaleta <bkaleta@student.42warsaw.pl>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 12:25:08 by bkaleta           #+#    #+#             */
/*   Updated: 2024/08/18 12:25:09 by bkaleta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_error(t_shell *shell, char *command, char *message, int status)
{
	ft_putstr_fd(command, STDERR_FILENO);
	ft_putstr_fd(message, STDERR_FILENO);
	safely_exit(status, shell, NULL, NULL);
}

/*
Clears the history, frees the input, the array and the cmd if they are not null,
	exits with EXIT_FAILURE
*/
void	throw_error_exit(char *input, char **array, t_cmd *cmd, t_var *var)
{
	rl_clear_history();
	if (input)
		free(input);
	if (array)
		free_array(array);
	if (cmd)
		free_cmd(cmd);
	if (var)
		free_var(var);
	exit(EXIT_FAILURE);
}
