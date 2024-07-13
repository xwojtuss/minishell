/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkornato <wkornato@student.42warsaw.p      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 20:39:37 by wkornato          #+#    #+#             */
/*   Updated: 2024/03/06 20:39:39 by wkornato         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	**free_array(char **ptr, int i)
{
	while (i > 0)
	{
		i--;
		free(ptr[i]);
	}
	free(ptr);
	return (0);
}

static int	count_words(char const *s, char c)
{
	int	count;

	count = 0;
	while (*s)
	{
		while (*s == c)
			s++;
		if (*s == '\0')
			break ;
		while (*s && *s != c)
			s++;
		count++;
	}
	return (count);
}

static int	count_letters(const char *s, char c)
{
	int	i;

	i = 0;
	while (s[i] && s[i] != c)
		i++;
	return (i);
}

static char	*create_word(char *s, char c, int *move)
{
	char	*elem;
	int		count;
	int		i;

	i = 0;
	*move = -42;
	while (s[i] == c)
		i++;
	count = count_letters((const char *)(s + i), c);
	elem = (char *)malloc((count + 1) * sizeof(char));
	if (!elem)
		return (NULL);
	while (s[i] && s[i] != c)
	{
		*elem = s[i];
		elem++;
		i++;
	}
	elem -= count;
	elem[count] = '\0';
	*move = i;
	return (elem);
}

/*Parameters s: The string to be split.
c: The delimiter character.
Return value The array of new strings resulting from the split.
NULL if the allocation fails.
External functs. malloc, free
Description Allocates (with malloc(3)) and returns an array
of strings obtained by splitting ’s’ using the
character ’c’ as a delimiter. The array must end
with a NULL pointer.*/
char	**ft_split(char const *s, char c)
{
	char			**array;
	char			*string;
	size_t			elem_number;
	unsigned int	i;
	int				move;

	i = 0;
	move = 0;
	string = (char *)s;
	elem_number = count_words(s, c);
	array = (char **)malloc((elem_number + 1) * sizeof(char *));
	if (!array)
		return (NULL);
	while (i < elem_number)
	{
		array[i] = create_word(string, c, &move);
		if (move == -42)
			return (free_array(array, i));
		string += move;
		i++;
	}
	array[elem_number] = NULL;
	return (array);
}
