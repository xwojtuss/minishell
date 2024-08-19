/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin_array.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkornato <wkornato@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 23:07:36 by wkornato          #+#    #+#             */
/*   Updated: 2024/08/19 11:40:51 by wkornato         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	get_length(char **array)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (array[i])
	{
		j += ft_strlen(array[i]);
		i++;
	}
	return (j);
}

char	*ft_strjoin_array(char **array)
{
	char	*result;
	int		i;
	int		j;

	i = 0;
	j = 0;
	result = (char *)ft_calloc(get_length(array) + 1, sizeof(char));
	if (!result)
		return (NULL);
	while (array[i])
	{
		ft_strlcpy(result + j, array[i], ft_strlen(array[i]) + 1);
		j += ft_strlen(array[i]);
		i++;
	}
	return (result);
}
