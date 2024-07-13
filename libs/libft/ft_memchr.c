/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkornato <wkornato@student.42warsaw.p      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 19:40:25 by wkornato          #+#    #+#             */
/*   Updated: 2024/03/05 19:40:27 by wkornato         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *str, int c, size_t n)
{
	unsigned char	*string;
	size_t			i;

	if (n <= 0 || !str)
		return (NULL);
	i = 0;
	string = (unsigned char *)str;
	while (i < n - 1 && *string != (unsigned char)c)
	{
		string++;
		i++;
	}
	if ((unsigned char)c == '\0')
		return (string);
	if (*string == '\0' || *string != (unsigned char)c)
		return (0);
	return (string);
}
