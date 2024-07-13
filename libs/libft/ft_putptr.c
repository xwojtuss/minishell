/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putptr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkornato <wkornato@student.42warsaw.p      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 19:06:27 by wkornato          #+#    #+#             */
/*   Updated: 2024/04/02 19:13:07 by wkornato         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_digits(unsigned long long int number)
{
	int	i;

	i = 0;
	while (number != 0)
	{
		i++;
		number /= 16;
	}
	return (i);
}

static void	fill_string(int length, unsigned long long int n, char *result)
{
	int	i;

	i = length - 1;
	while (n != 0)
	{
		if (n % 16 > 9)
			result[i] = 'a' + (n % 16) - 10;
		else
			result[i] = '0' + (n % 16);
		i--;
		n /= 16;
	}
	result[length] = '\0';
}

int	ft_putptr(void *ptr)
{
	char			*result;
	int				length;

	if (!ptr)
		return (ft_putstr("(nil)"));
	length = count_digits((unsigned long long int)ptr);
	result = (char *)malloc((1 + length) * sizeof(char));
	if (!result)
		return (-2147483648);
	fill_string(length, (unsigned long long int)ptr, result);
	ft_putstr("0x");
	ft_putstr(result);
	free(result);
	return (length + 2);
}
