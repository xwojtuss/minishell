#include "minishell.h"

bool	does_file_exist(char *path)
{
	int	fd;

	if (!path)
		return (false);
	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (false);
	close(fd);
	return (true);
}

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

char	*get_rid_of_quotes(char *path)
{
	char	*result;
	char	*temp;
	char	quotes;
	int		i;

	result = (char *)ft_calloc(PATH_MAX, sizeof(char));
	if (!result)
		return (NULL);
	temp = path;
	quotes = NOT_SET;
	i = 0;
	while (*temp)
	{
		if (*temp == '\'' && quotes == NOT_SET)
			quotes = '\'';
		else if (*temp == '\"' && quotes == NOT_SET)
			quotes = '\"';
		else if (*temp == quotes)
			quotes = NOT_SET;
		else
		{
			result[i] = *temp;
			i++;
		}
		temp++;
	}
	return (result);
}

char	*get_absolute_path(char *path)
{
	char	*temp;
	char	*result;
	char	*final;

	temp = get_rid_of_quotes(path);
	if (!temp)
		return (NULL);
	if (temp[0] == '/')
		return (temp);
	result = (char *)ft_calloc(PATH_MAX, sizeof(char));
	if (!result)
		return (free(temp), NULL);
	if (!getcwd(result, PATH_MAX))
	{
		free(result);
		return (NULL);
	}
	ft_strlcat(result, "/", PATH_MAX - ft_strlen(result));
	ft_strlcat(result, temp, PATH_MAX - ft_strlen(result));
	final = (char *)ft_calloc(PATH_MAX, sizeof(char));
	if (!final)
		return (free(temp), free(result), NULL);
	remove_dots(final, result);
	free(result);
	free(temp);
	return (final);
}
