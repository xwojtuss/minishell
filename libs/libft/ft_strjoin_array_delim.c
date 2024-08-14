/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin_array_delim.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkornato <wkornato@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 23:07:36 by wkornato          #+#    #+#             */
/*   Updated: 2024/08/15 00:28:08 by wkornato         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	get_length(char **array)
{
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (array[i])
	{
		j += ft_strlen(array[i]);
		i++;
	}
	return (j + i);
}

char	*ft_strjoin_array_delim(char **array, char delim)
{
	char	*result;
	int		i;
	int		j;

	i = 0;
	j = 0;
	result = (char *)ft_calloc(get_length(array), sizeof(char));
	if (!result)
		return (NULL);
	while (array[i])
	{
		ft_strlcpy(result + j, array[i], ft_strlen(array[i]) + 1);
		j += ft_strlen(array[i]);
		if (array[i + 1])
			result[j++] = delim;
		i++;
	}
	return (result);
}
