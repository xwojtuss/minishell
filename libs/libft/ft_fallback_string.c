/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fallback_string.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkornato <wkornato@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 23:22:56 by wkornato          #+#    #+#             */
/*   Updated: 2024/08/07 23:27:22 by wkornato         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
If the first string is NULL, return the second string
The second string does not need to be freed
*/
char	*ft_fallback_string(char *str, const char *fallback)
{
	char	*result;

	result = (char *)fallback;
	if (!str)
		return (result);
	return (str);
}
