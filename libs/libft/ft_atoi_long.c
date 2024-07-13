/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi_long.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkornato <wkornato@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 19:44:30 by wkornato          #+#    #+#             */
/*   Updated: 2024/06/04 19:05:28 by wkornato         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

long int	ft_atoi_long(const char *str)
{
	long int	result;
	int			change_sign;

	result = 0;
	while ((*str >= 7 && *str <= 13) || *str == 32)
		str++;
	change_sign = (*str == '-') * -2 + 1;
	if (*str == '-' || *str == '+')
		str++;
	while (*str && *str >= '0' && *str <= '9')
	{
		result *= 10;
		result += (*str - '0');
		str++;
	}
	return (change_sign * result);
}
