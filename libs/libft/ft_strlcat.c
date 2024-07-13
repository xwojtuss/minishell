/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkornato <wkornato@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 18:27:54 by wkornato          #+#    #+#             */
/*   Updated: 2024/02/27 23:10:13 by wkornato         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dest, const char *src, unsigned int size)
{
	size_t			copied;
	unsigned int	count;
	unsigned int	i;

	count = ft_strlen(dest);
	i = 0;
	copied = ft_strlen(src);
	if (size <= count)
		return (copied += size);
	else
		copied += count;
	while (src[i] != '\0' && (count + 1) < size)
	{
		dest[count] = src[i];
		i++;
		count++;
	}
	dest[count] = '\0';
	return (copied);
}
