#include "minishell.h"

char	*remove_dots(char *final, char *path)
{
	char	*token;
	char	*rest;
	char	temp[PATH_MAX] = {0};
	char	*last_slash;

	rest = path;
	last_slash = NULL;
	while (true)
	{
		token = ft_strtok_r(rest, "/", &rest);
		if (!token)
			break ;
		if (ft_strcmp(token, ".") == 0)
			continue ;
		else if (ft_strcmp(token, "..") == 0)
		{
			if (last_slash)
			{
				*last_slash = '\0';
				last_slash = ft_strrchr(temp, '/');
			}
		}
		else
		{
			ft_strlcat(temp, "/", PATH_MAX - strlen(temp));
			ft_strlcat(temp, token, PATH_MAX - strlen(temp));
			last_slash = ft_strrchr(temp, '/');
		}
	}
	ft_strlcpy(final, temp, PATH_MAX);
	return (final);
}

char	*get_absolute_path(char *path)
{
	char	*result;
	char	*final;

	if (path[0] == '/')
		return (ft_strdup(path));
	result = (char *)ft_calloc(PATH_MAX, sizeof(char));
	if (!result)
		return (NULL);
	if (!getcwd(result, PATH_MAX))
	{
		free(result);
		return (NULL);
	}
	ft_strlcat(result, "/", PATH_MAX - ft_strlen(result));
	ft_strlcat(result, path, PATH_MAX - ft_strlen(result));
	final = (char *)ft_calloc(PATH_MAX, sizeof(char));
	if (!final)
		return (NULL);
	remove_dots(final, result);
	free(result);
	return (final);
}

t_files	*add_file(char *file, int fd, t_files *files)
{
	t_files	*new;

	new = (t_files *)malloc(sizeof(t_files));
	if (!new)
		return (NULL);
	new->path = get_absolute_path(file);
	if (!new->path)
		return (NULL);
	new->fd = fd;
	new->next = files;
	return (new);
}

int	get_fd(char *file, t_files *files)
{
	while (files)
	{
		if (!ft_strcmp(files->path, file))
			return (files->fd);
		files = files->next;
	}
	return (NOT_SET);
}
