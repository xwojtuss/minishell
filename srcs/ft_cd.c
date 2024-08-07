/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkornato <wkornato@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 23:46:58 by wkornato          #+#    #+#             */
/*   Updated: 2024/08/08 00:12:39 by wkornato         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_cd(int argc, char **argv)//maybe it should also modify the env variable PWD
{
	char *path;
	
	if (argc != 2)
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	path = get_absolute_path(argv[1]);
	if (chdir(path) != 0)
	{
		free(path);
		ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
		ft_putstr_fd(argv[1], STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		perror("");
		return (EXIT_FAILURE);
	}
	free(path);
	return (EXIT_SUCCESS);
}