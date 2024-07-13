/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkornato <wkornato@student.42warsaw.p      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 19:43:20 by wkornato          #+#    #+#             */
/*   Updated: 2024/03/05 19:43:25 by wkornato         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*Parameters s: The string from which to create the substring.
start: The start index of the substring in the
string ’s’.
len: The maximum length of the substring.
Return value The substring.
NULL if the allocation fails.
External functs. malloc
Description Allocates (with malloc(3)) and returns a substring
from the string ’s’.
The substring begins at index ’start’ and is of
maximum size ’len’.*/
char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	unsigned int	i;
	char			*string;
	size_t			length;

	if (!s)
		return (NULL);
	length = ft_strlen(s);
	if ((start >= length) || (len == 0))
		return (ft_calloc(1, sizeof(char)));
	if ((length - start) < len)
		length -= start;
	else
		length = len;
	string = (char *)malloc(sizeof(char) * (length + 1));
	if (!string)
		return (NULL);
	i = 0;
	while (i < length)
	{
		string[i] = s[start];
		i++;
		start++;
	}
	string[i] = '\0';
	return (string);
}
