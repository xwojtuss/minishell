/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_files.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkaleta <bkaleta@student.42warsaw.pl>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 12:35:01 by bkaleta           #+#    #+#             */
/*   Updated: 2024/08/18 12:35:03 by bkaleta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	open_temp_file_two(t_cmd *cmd)
{
	free(cmd->write_path);
	cmd->write_path = get_absolute_path("./.minishell_empty_file_two");
	if (!cmd->write_path)
		return (0);
	cmd->write_fd = open(cmd->write_path, O_RDONLY | O_CREAT | O_TRUNC, 0644);
	if (cmd->write_fd == -1)
		return (0);
	return (1);
}

int	open_temp_file(t_cmd *cmd)
{
	free(cmd->read_path);
	cmd->read_path = get_absolute_path("./.minishell_empty_file");
	if (!cmd->read_path)
		return (0);
	cmd->read_fd = open(cmd->read_path, O_RDONLY | O_CREAT | O_TRUNC, 0644);
	if (cmd->read_fd == -1)
		return (0);
	return (1);
}

int	open_normal_file(t_cmd *cmd)
{
	cmd->read_fd = open(cmd->read_path, O_RDONLY);
	if (cmd->read_fd == -1)
	{
		ft_putstr_fd(cmd->read_path, STDERR_FILENO);
		ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
		return (1);
	}
	return (1);
}
