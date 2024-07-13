/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkornato <wkornato@student.42warsaw.p      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 17:33:38 by wkornato          #+#    #+#             */
/*   Updated: 2024/03/06 17:33:40 by wkornato         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	prepare_result(int n, char *result, size_t count)
{
	if (n < 0)
		result[0] = '-';
	else if (n == 0)
		result[0] = '0';
	result[count] = '\0';
}

/*Parameters n: the integer to convert.
Return value The string representing the integer.
NULL if the allocation fails.
External functs. malloc
Description Allocates (with malloc(3)) and returns a string
representing the integer received as an argument.
Negative numbers must be handled.*/
char	*ft_itoa(int n)
{
	int			is_negative;
	long int	number;
	char		*result;
	size_t		count;

	is_negative = (n <= 0);
	count = is_negative;
	number = n;
	while (number != 0)
	{
		number /= 10;
		count++;
	}
	result = (char *)malloc((count + 1) * sizeof(char));
	if (!result)
		return (NULL);
	prepare_result(n, result, count);
	number = n;
	number *= (!is_negative * 2 - 1);
	while (number != 0)
	{
		result[--count] = (number % 10) + '0';
		number /= 10;
	}
	return (result);
}
