/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkornato <wkornato@student.42warsaw.p      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 19:51:26 by wkornato          #+#    #+#             */
/*   Updated: 2024/03/05 19:51:27 by wkornato         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	count;

	count = -1;
	while (src[++count] != '\0' && size > (count + 1))
		dst[count] = src[count];
	if (size > 0)
		dst[count] = '\0';
	count = 0;
	while (src[count])
		count++;
	return (count);
}