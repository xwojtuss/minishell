/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkornato <wkornato@student.42warsaw.p      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 19:49:48 by wkornato          #+#    #+#             */
/*   Updated: 2024/03/05 19:49:50 by wkornato         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	char	*duplicate;
	size_t	length;

	length = ft_strlen(s);
	duplicate = (char *)ft_calloc(length + 1, sizeof(char));
	if (!(duplicate))
		return (NULL);
	while (*s != '\0')
	{
		*duplicate = *s;
		s++;
		duplicate++;
	}
	*duplicate = '\0';
	return (duplicate - length);
}
