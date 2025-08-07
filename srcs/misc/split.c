/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkornato <wkornato@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 12:37:18 by bkaleta           #+#    #+#             */
/*   Updated: 2024/09/05 14:51:52 by wkornato         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*check_count_quotes(int *quote, char *s, int *count)
{
	while (*s && !ft_iswhite(*s) && *s != '\'' && *s != '\"')
		s++;
	if (ft_iswhite(*s) && *s && *quote == NOT_SET)
	{
		(*count)++;
		while (ft_iswhite(*s) && *s)
			s++;
	}
	else if (*s == '\'' || *s == '\"')
	{
		*quote = *s;
		s++;
		while (*s && *s != *quote)
			s++;
		if (!*s)
			return (s);
		s++;
		*quote = NOT_SET;
	}
	else if (*s)
		s++;
	return (s);
}

static int	count_words(char const *s)
{
	char	*temp;
	int		count;
	int		quote;

	while (ft_iswhite(*s) && *s)
		s++;
	if (*s == '\0')
		return (0);
	count = 1;
	quote = NOT_SET;
	temp = (char *)s;
	while (*temp)
		temp = check_count_quotes(&quote, temp, &count);
	return (count);
}

static void	check_quotes(int *wait_for, char c)
{
	if (c == '\"' && *wait_for == NOT_SET)
		*wait_for = '\"';
	else if (c == '\'' && *wait_for == NOT_SET)
		*wait_for = '\'';
	else if (c == *wait_for)
		*wait_for = NOT_SET;
}

static int	copy_data(const char *str, char ***result, char *buffer,
		int buf_index)
{
	int	wait_for;
	int	size;

	size = 0;
	wait_for = NOT_SET;
	while (*str)
	{
		check_quotes(&wait_for, *str);
		if (ft_iswhite(*str) && *str && wait_for == NOT_SET && buf_index > 0)
		{
			if (!copy_word(buffer, &buf_index, &result, &size))
				return (0);
		}
		else if (*str && !(ft_iswhite(*str) && *str && wait_for == NOT_SET))
			buffer[buf_index++] = *str;
		str++;
	}
	if (buf_index > 0)
		if (!copy_word(buffer, &buf_index, &result, &size))
			return (0);
	return (1);
}

char	**split_args(const char *str)
{
	char	**result;
	char	*buffer;
	int		buf_index;

	result = NULL;
	buffer = NULL;
	buf_index = 0;
	buffer = (char *)ft_calloc(ft_strlen(str) + 1, sizeof(char));
	if (!buffer)
		return (0);
	result = (char **)ft_calloc(count_words(str) + 1, sizeof(char *));
	if (!result)
		return (free(buffer), NULL);
	if (!copy_data(str, &result, buffer, buf_index))
		return (free(buffer), free_array(result), NULL);
	result[count_words(str)] = NULL;
	free(buffer);
	return (result);
}
