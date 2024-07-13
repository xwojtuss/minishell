/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkornato <wkornato@student.42warsaw.p      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 21:13:37 by wkornato          #+#    #+#             */
/*   Updated: 2024/04/02 21:13:39 by wkornato         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	expand_str(int *buffer, char **result)
{
	char	*temp;
	int		j;

	*buffer *= 2;
	temp = (char *)malloc(sizeof(char) * (*buffer + 1));
	if (!temp)
	{
		*result = NULL;
		free(result);
		return ;
	}
	j = 0;
	while (j < (*buffer / 2))
	{
		temp[j] = (*result)[j];
		j++;
	}
	free(*result);
	*result = temp;
	return ;
}

static char	*prep(int fd, int *buffer)
{
	char	*result;

	*buffer = BUFFER_SIZE;
	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	result = (char *)malloc(sizeof(char) * (*buffer + 1));
	if (!result)
		return (NULL);
	return (result);
}

static char	*read_byte(int fd, char *result, int *i)
{
	int	bytes_read;

	bytes_read = read(fd, &(result[*i]), 1);
	if (bytes_read == -1 || (bytes_read == 0 && *i == 0))
	{
		free(result);
		return (NULL);
	}
	else if ((bytes_read == 0 && *i != 0) || result[*i] == '\n')
	{
		*i += (bytes_read == 1);
		result[*i] = '\0';
		return (result);
	}
	return (result);
}

char	*get_next_line(int fd)
{
	static int	nothingness;
	char		*result;
	int			i;
	int			buffer;

	(void)nothingness;
	result = prep(fd, &buffer);
	if (!result)
		return (NULL);
	i = 0;
	while (1)
	{
		result = read_byte(fd, result, &i);
		if (!result || result[i] == '\0')
			return (result);
		i++;
		if (i >= buffer - 1)
		{
			expand_str(&buffer, &result);
			if (!result)
				break ;
		}
	}
	free(result);
	return (NULL);
}
