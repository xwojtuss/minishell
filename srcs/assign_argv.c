/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assign_argv.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkornato <wkornato@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/25 13:49:23 by wkornato          #+#    #+#             */
/*   Updated: 2024/08/25 13:49:24 by wkornato         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*copy_without_quotes(char *s)
{
	char	*result;
	int		wait_for;
	int		i;

	result = (char *)ft_calloc(ft_strlen(s) + 1, sizeof(char));
	if (!result)
		return (NULL);
	wait_for = NOT_SET;
	i = 0;
	while (*s)
	{
		if (*s == '\'' && wait_for == NOT_SET)
			wait_for = '\'';
		else if (*s == '\"' && wait_for == NOT_SET)
			wait_for = '\"';
		else if (*s == wait_for)
			wait_for = NOT_SET;
		else
		{
			result[i] = *s;
			i++;
		}
		s++;
	}
	return (result);
}

int	copy_until_pipe(char **array, t_cmd **new, int i)
{
	int	j;

	j = 0;
	while (array[i] && ft_strcmp(array[i], "|"))
	{
		if (is_redirect(array[i]))
		{
			if (!set_redirect(array[i], new, array[i + 1]))
				return (0);
			i++;
		}
		else
		{
			(*new)->argv[j] = copy_without_quotes(array[i]);
			if (!((*new)->argv[j]))
				return (0);
			j++;
		}
		if (array[i] && ft_strcmp(array[i], "|"))
			i++;
	}
	return (1);
}

int	assign_argv(char **array, t_cmd **new, int command)
{
	int	i;

	i = command;
	while (array[i] && ft_strcmp(array[i], "|"))
	{
		if (is_redirect(array[i]))
			i++;
		else
			(*new)->argc++;
		if (array[i] && ft_strcmp(array[i], "|"))
			i++;
	}
	(*new)->argv = (char **)ft_calloc((*new)->argc + 1, sizeof(char *));
	if (!((*new)->argv))
		return (0);
	(*new)->argv[(*new)->argc] = NULL;
	return (copy_until_pipe(array, new, command));
}
