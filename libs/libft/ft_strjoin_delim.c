/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin_delim.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkornato <wkornato@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 19:49:32 by wkornato          #+#    #+#             */
/*   Updated: 2024/08/12 21:08:48 by wkornato         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*Parameters s1: The prefix string.
s2: The suffix string.
delim: The delimiter char.
Return value The new string.
NULL if the allocation fails.
External functs. malloc
Description Allocates (with malloc(3)) and returns a new
string, which is the result of the concatenation
of ’s1’, char delim, ’s2’.*/
char	*ft_strjoin_delim(char const *s1, char delim, char const *s2)
{
	size_t	length;
	char	*string;

	length = ft_strlen(s1) + ft_strlen(s2) + 1;
	string = (char *)malloc((length + 1) * sizeof(char));
	if (!string)
		return (NULL);
	while (*s1)
	{
		*string = *s1;
		string++;
		s1++;
	}
	*string = delim;
	string++;
	while (*s2)
	{
		*string = *s2;
		string++;
		s2++;
	}
	*string = '\0';
	return (string - length);
}
