/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkornato <wkornato@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 18:25:00 by bkaleta           #+#    #+#             */
/*   Updated: 2024/08/19 11:42:24 by wkornato         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	history_init(t_shell *shell)
{
	shell->history = malloc(sizeof(t_history));
	if (!shell->history)
		throw_error_exit(NULL, NULL, NULL, NULL);
	shell->history->last_cmd = NULL;
}

void	update_last_command(t_history *history, char *new_command)
{
	if (history->last_cmd)
		free(history->last_cmd);
	history->last_cmd = ft_strdup(new_command);
	if (!history->last_cmd)
		throw_error_exit(NULL, NULL, NULL, NULL);
}

void	free_history(t_shell *shell)
{
	if (shell->history)
	{
		if (shell->history->last_cmd)
		{
			free(shell->history->last_cmd);
			shell->history->last_cmd = NULL;
		}
		free(shell->history);
		shell->history = NULL;
	}
}
