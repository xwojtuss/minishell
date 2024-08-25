/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkornato <wkornato@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 12:36:49 by bkaleta           #+#    #+#             */
/*   Updated: 2024/08/25 13:41:50 by wkornato         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_redirect(char *str)
{
	if (!ft_strcmp(str, ">") || !ft_strcmp(str, ">>") || !ft_strcmp(str, "<<")
		|| !ft_strcmp(str, "<"))
		return (true);
	return (false);
}

int	redir_input(char *file, t_cmd *cmd)
{
	cmd->read_fd = open(file, O_RDONLY);
	if (cmd->read_fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(file, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		return (0);
	}
	return (1);
}

int	redir_output(char *file, t_cmd *cmd)
{
	cmd->write_fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (cmd->write_fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(file, 2);
		ft_putstr_fd(": Error creating or opening file\n", 2);
		return (0);
	}
	return (1);
}

int	redir_append(char *file, t_cmd *cmd)
{
	cmd->write_fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (cmd->write_fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(file, 2);
		ft_putstr_fd(": Error creating or opening file\n", 2);
		return (0);
	}
	return (1);
}

int	redir_delimiter(char *delim)
{
	char	buffer[BUFFER_SIZE + 1];
	int		bytes;

	while (true)
	{
		printf("> ");
		bytes = read(0, buffer, BUFFER_SIZE);
		if (bytes == -1)
		{
			ft_putstr_fd("minishell: read error\n", 2);
			return (0);
		}
		if (!ft_strncmp(buffer, delim, ft_strlen(delim)))
			break ;
		printf("buffer: %s\n", buffer);
	}
	return (1);
}
