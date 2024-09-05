/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkornato <wkornato@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 12:35:09 by bkaleta           #+#    #+#             */
/*   Updated: 2024/09/05 14:14:15 by wkornato         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	double_dot(char *temp, char *token, char **last_slash)
{
	if (ft_strcmp(token, "..") == 0)
	{
		if (*last_slash && *last_slash != temp)
		{
			**last_slash = '\0';
			*last_slash = ft_strrchr(temp, '/');
		}
		else
		{
			temp[0] = '/';
			temp[1] = '\0';
			*last_slash = temp;
		}
	}
	else
	{
		ft_strlcat(temp, "/", PATH_MAX - strlen(temp));
		ft_strlcat(temp, token, PATH_MAX - strlen(temp));
		*last_slash = ft_strrchr(temp, '/');
	}
}

static char	*remove_dots(char *final, char *path)
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

static char	*get_rid_of_quotes(char *path)
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
		if ((*temp == '\'' || *temp == '\"') && quotes == NOT_SET)
			quotes = *temp;
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
	final = remove_dots(final, result);
	free(result);
	free(temp);
	return (final);
}
