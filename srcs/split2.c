/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkaleta <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 22:02:03 by bkaleta           #+#    #+#             */
/*   Updated: 2024/08/16 22:02:36 by bkaleta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	add_string(char ****array, int *size, const char *str)
{
	(**array)[*size] = ft_strdup(str);
	if (!(**array)[*size])
		return (0);
	(*size)++;
	return (1);
}

int	copy_word(char *buffer, int *buf_index, char ****result, int *size)
{
	buffer[*buf_index] = '\0';
	if (!add_string(result, size, buffer))
		return (0);
	*buf_index = 0;
	return (1);
}