/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkornato <wkornato@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 00:31:08 by wkornato          #+#    #+#             */
/*   Updated: 2024/08/15 00:31:09 by wkornato         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_empty_or_pipe(char *str)
{
	if (!str)
		return (true);
	if (!*str || !ft_strcmp(str, "|"))
		return (true);
	return (false);
}

bool	check_redirects(t_shell *shell, char **array)
{
	int	i;

	i = 0;
	while (array[i])
	{
		if ((ft_strcmp(array[i], "<") == 0 || ft_strcmp(array[i], ">") == 0
				|| ft_strcmp(array[i], ">>") == 0 || ft_strcmp(array[i],
					"<<") == 0) && !array[i + 1])
			ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n",
				STDERR_FILENO);
		else if (!ft_strcmp(array[i], ">>>"))
			ft_putstr_fd("minishell: syntax error near unexpected token `>'\n",
				STDERR_FILENO);
		else
		{
			i++;
			continue ;
		}
		set_last_exit_code(shell->var, 2);
		free_array(array);
		return (false);
	}
	return (true);
}
