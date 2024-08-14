/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkornato <wkornato@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 19:49:48 by wkornato          #+#    #+#             */
/*   Updated: 2024/08/15 00:27:38 by wkornato         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strndup(const char *s, int length)
{
	char	*duplicate;
	int		i;

	if (length < 0)
		return (NULL);
	duplicate = (char *)ft_calloc(length + 1, sizeof(char));
	if (!(duplicate))
		return (NULL);
	i = 0;
	while (i < length)
	{
		duplicate[i] = *s;
		if (*s == '\0')
			break ;
		s++;
		i++;
	}
	return (duplicate);
}
