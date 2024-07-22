/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchrs.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkornato <wkornato@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 19:50:33 by wkornato          #+#    #+#             */
/*   Updated: 2024/07/21 01:18:51 by wkornato         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
If the chars are not found it returns a pointer to the null terminator instead
*/
char	*ft_strchrs_mf(const char *str, const char *c)
{
	char	*ptr;
	char	*chrs;

	ptr = (char *)str;
	while (*ptr)
	{
		chrs = (char *)c;
		while (*chrs)
		{
			if (*ptr == *chrs)
				return (ptr);
			chrs++;
		}
		ptr++;
	}
	if (*ptr == '\0')
		return (ptr);
	return (NULL);
}

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
