/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkaleta <bkaleta@student.42warsaw.pl>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 12:28:29 by bkaleta           #+#    #+#             */
/*   Updated: 2024/08/18 12:28:31 by bkaleta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_pwd(int argc, char **argv)
{
	char	*curr_work_dir;

	(void)argc;
	(void)argv;
	curr_work_dir = getcwd(NULL, 0);
	if (!curr_work_dir)
	{
		ft_putstr_fd("minishell: pwd: ", STDERR_FILENO);
		ft_putstr_fd(strerror(errno), STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	ft_putendl_fd(curr_work_dir, STDOUT_FILENO);
	free(curr_work_dir);
	return (EXIT_SUCCESS);
}
