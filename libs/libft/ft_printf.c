/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkornato <wkornato@student.42warsaw.p      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/02 19:04:38 by wkornato          #+#    #+#             */
/*   Updated: 2024/04/02 19:04:42 by wkornato         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	is_data_type(char letter)
{
	if (letter == 'c' || letter == 's' || letter == 'p' || letter == 'd'
		|| letter == 'i' || letter == 'u' || letter == 'x' || letter == 'X'
		|| letter == '%')
		return (1);
	return (0);
}

static int	print_this_type(char type, va_list args)
{
	int	printed;

	printed = 0;
	if (type == 'c')
		printed += ft_putchar(va_arg(args, int));
	else if (type == 's')
		printed += ft_putstr(va_arg(args, char *));
	else if (type == 'i')
		ft_putnbr(va_arg(args, int), &printed, 1);
	else if (type == '%')
		printed += ft_putchar(37);
	else if (type == 'x')
		ft_puthex(va_arg(args, int), &printed, 'a');
	else if (type == 'X')
		ft_puthex(va_arg(args, int), &printed, 'A');
	else if (type == 'd')
		ft_putnbr(va_arg(args, int), &printed, 1);
	else if (type == 'u')
		ft_putnbr(va_arg(args, int), &printed, 0);
	else if (type == 'p')
		printed += ft_putptr(va_arg(args, void *));
	if (printed < 0)
		return (-2147483648);
	return (printed);
}

int	ft_printf(const char *str, ...)
{
	int		printed;
	va_list	args;

	va_start(args, str);
	printed = 0;
	while (*str)
	{
		if (*str == '%')
		{
			str++;
			while (*str && !is_data_type(*str))
				str++;
			if (*str)
				printed += print_this_type(*str, args);
		}
		else
			printed += write(1, str, 1);
		if (*str)
			str++;
	}
	va_end(args);
	if (printed < 0)
		return (-1);
	return (printed);
}
