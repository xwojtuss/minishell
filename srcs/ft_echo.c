/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkaleta <bkaleta@student.42warsaw.pl>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 23:58:10 by wkornato          #+#    #+#             */
/*   Updated: 2024/08/18 12:27:06 by bkaleta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_argv(char **argv, int i, int new_line)
{
	while (argv[i])
	{
		ft_putstr_fd(argv[i], STDOUT_FILENO);
		if (argv[i + 1])
			ft_putstr_fd(" ", STDOUT_FILENO);
		i++;
	}
	if (new_line)
		ft_putstr_fd("\n", STDOUT_FILENO);
}

int	ft_echo(int argc, char **argv)
{
	int	j;
	int	i;
	int	new_line;

	(void)argc;
	i = 1;
	new_line = 1;
	while (argv[1] && ft_strncmp(argv[i], "-n", 2) == 0)
	{
		j = 2;
		while (argv[i][j] == 'n')
			j++;
		if (argv[i++][j] == '\0')
			new_line = 0;
		else
			break ;
	}
	print_argv(argv, i, new_line);
	return (EXIT_SUCCESS);
}
