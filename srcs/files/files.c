/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   files.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkaleta <bkaleta@student.42warsaw.pl>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 12:25:56 by bkaleta           #+#    #+#             */
/*   Updated: 2024/08/18 12:25:58 by bkaleta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
Checks if the file is in the specified path,
there can be multiple paths separated by ':'
if the file is found, it returns the full path
*/
char	*locate_file(char *command, char *path)
{
	char	**dirs;
	char	*result;
	char	*temp;

	if (!path || !command)
		return (NULL);
	dirs = ft_split(path, ':');
	if (!dirs)
		return (NULL);
	temp = get_path_of_file(dirs, command);
	if (!temp)
		return (free_array(dirs), NULL);
	result = ft_strjoin(temp, "/");
	if (!result)
		return (free_array(dirs), NULL);
	temp = result;
	result = ft_strjoin(temp, command);
	free(temp);
	free_array(dirs);
	return (result);
}

bool	check_path_for_file(char *path, char *command)
{
	DIR				*dir;
	struct dirent	*info;

	dir = opendir(path);
	if (!dir)
		return (false);
	while (true)
	{
		info = readdir(dir);
		if (!info)
			break ;
		if (!ft_strcmp(info->d_name, command))
		{
			closedir(dir);
			return (true);
		}
	}
	closedir(dir);
	return (false);
}

char	*get_path_of_file(char **dirs, char *command)
{
	int	i;

	i = 0;
	while (dirs[i])
	{
		if (check_path_for_file(dirs[i], command))
			break ;
		i++;
	}
	return (dirs[i]);
}

/*
returns the status of the file
1 - a directory
2 - no execute permission
4 - no read permission
8 - no write permission
128 - file does not exist
*/
int	check_file(char *path)
{
	struct stat	info;
	int			status;

	status = 0;
	if (access(path, F_OK) != 0)
		return (128);
	if (stat(path, &info) != 0)
		return (128);
	if (S_ISDIR(info.st_mode))
		status |= 1;
	if (access(path, X_OK) != 0)
		status |= 2;
	if (access(path, R_OK) != 0)
		status |= 4;
	if (access(path, W_OK) != 0)
		status |= 8;
	return (status);
}

bool	does_file_exist(char *path)
{
	if (!path)
		return (false);
	if (access(path, F_OK) == 0)
		return (true);
	return (false);
}
