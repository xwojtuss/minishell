/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_redirect.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkornato <wkornato@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/25 15:21:46 by wkornato          #+#    #+#             */
/*   Updated: 2024/08/25 15:25:05 by wkornato         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	set_write_redirect(t_cmd **cmd, char *file, int mode)
{
	if ((*cmd)->write_path)
		free((*cmd)->write_path);
	(*cmd)->write_mode = mode;
	(*cmd)->write_path = get_absolute_path(file);
	if ((*cmd)->write_path && does_file_exist((*cmd)->write_path)
		&& check_file((*cmd)->write_path) & 8)
	{
		ft_putstr_fd(file, STDERR_FILENO);
		ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
		return (0);
	}
	if ((*cmd)->write_path)
		close(open((*cmd)->write_path, O_WRONLY | O_CREAT, 0644));
	return (1);
}

int	set_read_redirect(t_cmd	**cmd, char *file)
{
	(*cmd)->read_path = get_absolute_path(file);
	if (!does_file_exist((*cmd)->read_path))
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(file, STDERR_FILENO);
		ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
		return (0);
	}
	return (1);
}

int	set_redirect(char *str, t_cmd **cmd, char *file)
{
	t_cmd	*new;

	if (!ft_strcmp(str, ">"))
	{
		if (!set_write_redirect(cmd, file, MODE_WRITE))
			return (0);
	}
	else if (!ft_strcmp(str, ">>"))
	{
		if (!set_write_redirect(cmd, file, MODE_APPEND))
			return (0);
	}
	else if (!ft_strcmp(str, "<<"))
	{
		new = read_stdin_delim(file);
		if (!new || !new->argv || !new->argv[0])
			return (0);
		new->next = *cmd;
		*cmd = new;
	}
	else if (!ft_strcmp(str, "<") && !set_read_redirect(cmd, file))
		return (0);
	return (1);
}

int	redirect(char *str, char *file, t_cmd *cmd, t_shell *shell)
{
	int	status;

	if (file == NULL)
	{
		ft_putstr_fd("minishell: syntax error ", STDERR_FILENO);
		ft_putstr_fd("near unexpected token `newline'\n", STDERR_FILENO);
		set_last_exit_code(shell->var, 2);
		return (0);
	}
	if (ft_strcmp(str, ">"))
		status = redir_input(file, cmd);
	else if (ft_strcmp(str, ">>"))
		status = redir_append(file, cmd);
	else if (ft_strcmp(str, "<"))
		status = redir_output(file, cmd);
	else
		status = redir_delimiter(file);
	if (!status)
		return (0);
	return (1);
}
