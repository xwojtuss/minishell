/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchrs.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkornato <wkornato@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 19:50:33 by wkornato          #+#    #+#             */
/*   Updated: 2024/07/19 21:11:02 by wkornato         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchrs(const char *str, const char *c)
{
	char	*ptr;
	char	*chrs;

	ptr = (char *)str;
	while (*ptr)
	{
		chrs = (char *)c;
		while (*chrs)
		{
			if (*ptr == *chrs || *ptr == '\0')
				return (ptr);
			chrs++;
		}
		ptr++;
	}
	return (NULL);
}
