/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcspn.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkornato <wkornato@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 00:27:50 by wkornato          #+#    #+#             */
/*   Updated: 2024/08/15 00:27:53 by wkornato         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
Calculates the length of the number of characters before the 1st occurrence
of character present in both the string.
*/
size_t	ft_strcspn(const char *str1, const char *str2)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (str1[i])
	{
		j = 0;
		while (str2[j])
		{
			if (str1[i] == str2[j])
				return (i);
			j++;
		}
		i++;
	}
	return (i);
}
