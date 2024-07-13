/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_put.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkornato <wkornato@student.42warsaw.p      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 19:04:57 by wkornato          #+#    #+#             */
/*   Updated: 2024/04/02 19:06:02 by wkornato         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_putchar(int to_print)
{
	char	letter;

	letter = (unsigned char)to_print;
	write(1, &letter, 1);
	return (1);
}

int	ft_puthex(long int n, int *count, char offset)
{
	char	digit;

	if (n < 0)
	{
		ft_puthex(4294967296 + n, count, offset);
		return (*count);
	}
	if (n > 15)
		ft_puthex(n / 16, count, offset);
	if (n % 16 > 9)
		digit = offset + (n % 16) - 10;
	else
		digit = '0' + (n % 16);
	write(1, &digit, 1);
	return (++(*count));
}

int	ft_putstr(char *string)
{
	int	count;

	if (!string)
		return (ft_putstr("(null)"));
	count = 0;
	while (string[count])
		count++;
	write(1, string, count);
	return (count);
}

int	ft_putnbr(long int n, int *count, int is_signed)
{
	char	digit;

	if (n < 0 && is_signed)
	{
		n *= -1;
		write(1, "-", 1);
		*count = 1;
	}
	else if (n < 0 && !is_signed)
	{
		ft_putnbr(4294967296 + n, count, is_signed);
		return (*count);
	}
	if (n > 9)
		ft_putnbr(n / 10, count, is_signed);
	digit = '0' + (n % 10);
	write(1, &digit, 1);
	return (++(*count));
}
