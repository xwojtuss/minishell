#include "minishell.h"

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

static void	double_dot(char *temp, char *token, char **last_slash)
{
	if (ft_strcmp(token, "..") == 0)
	{
		if (*last_slash)
		{
			**last_slash = '\0';
			*last_slash = ft_strrchr(temp, '/');
		}
	}
	else
	{
		ft_strlcat(temp, "/", PATH_MAX - strlen(temp));
		ft_strlcat(temp, token, PATH_MAX - strlen(temp));
		*last_slash = ft_strrchr(temp, '/');
	}
}

char	*remove_dots(char *final, char *path)
{
	char	*token;
	char	*rest;
	char	temp[PATH_MAX];
	char	*last_slash;

	ft_bzero(temp, PATH_MAX);
	rest = path;
	last_slash = NULL;
	while (true)
	{
		token = ft_strtok_r(rest, "/", &rest);
		if (!token)
			break ;
		if (ft_strcmp(token, ".") == 0)
			continue ;
		else
			double_dot(temp, token, &last_slash);
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

	if (!path)
		return (NULL);
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
			result[i++] = *temp;
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
		return (free(result), NULL);
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
