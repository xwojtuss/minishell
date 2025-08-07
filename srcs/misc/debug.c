/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkaleta <bkaleta@student.42warsaw.pl>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 12:24:12 by bkaleta           #+#    #+#             */
/*   Updated: 2024/08/18 12:24:14 by bkaleta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
Prints the array of strings, the array must be NULL terminated
*/
void	print_array(char **array)
{
	int	i;

	i = 0;
	while (array[i])
	{
		printf("array[%d]: %s\n", i, array[i]);
		i++;
	}
}

void	print_cmd(t_cmd *cmd)
{
	t_cmd	*tmp;
	int		i;

	i = 0;
	tmp = cmd;
	while (tmp)
	{
		printf("cmd %d\n", i++);
		printf("argc: %d\n", tmp->argc);
		printf("read_fd: %d\n", tmp->read_fd);
		printf("write_fd: %d\n", tmp->write_fd);
		printf("write_mode: %d\n", tmp->write_mode);
		printf("read_path: %s\n", tmp->read_path);
		printf("write_path: %s\n", tmp->write_path);
		print_array(tmp->argv);
		tmp = tmp->next;
	}
}
