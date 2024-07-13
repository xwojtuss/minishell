/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkornato <wkornato@student.42warsaw.p      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 19:47:04 by wkornato          #+#    #+#             */
/*   Updated: 2024/03/05 19:47:05 by wkornato         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *str, int ch)
{
	int		i;
	char	*ptr;

	ptr = NULL;
	i = -1;
	if ((char)ch == '\0')
		return (ft_strchr(str, '\0'));
	while (str[++i])
		if (str[i] == (char)ch)
			ptr = (char *)(str + i);
	return (ptr);
}
