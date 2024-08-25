/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkornato <wkornato@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 12:23:03 by bkaleta           #+#    #+#             */
/*   Updated: 2024/08/25 15:55:32 by wkornato         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*alloc_cmd(int *i, int *command, char *read_path)
{
	t_cmd	*new;

	new = (t_cmd *)ft_calloc(1, sizeof(t_cmd));
	if (!new)
		return (NULL);
	new->argv = NULL;
	if (read_path)
		new->read_path = ft_strdup(read_path);
	else
		new->read_path = NULL;
	new->read_fd = NOT_SET;
	new->write_fd = NOT_SET;
	new->write_mode = NOT_SET;
	new->write_path = NULL;
	new->next = NULL;
	if (i && command)
		*command = *i;
	new->argc = 0;
	return (new);
}

static void	check_for_spaces(char *input, int *length, int *i,
		bool is_in_quotes)
{
	if (i > 0 && input[*i] == '|' && input[*i - 1] != ' ' && !is_in_quotes)
		(*length)++;
	if (input[*i] == '|' && input[*i + 1] != ' ' && input[*i + 1] != '\0'
		&& !is_in_quotes)
		(*length)++;
	if (input[*i] == '<' || input[*i] == '>')
		(*length) += 2;
	(*length)++;
	(*i)++;
}

int	count_length(char *input, t_var *var, int *length)
{
	bool	is_in_quotes;
	char	*name;
	int		i;

	i = 0;
	is_in_quotes = false;
	while (input[i])
	{
		if (input[i] == '\'')
			is_in_quotes = !is_in_quotes;
		if (input[i] == '$' && !is_in_quotes && input[i + 1] != ' ' && input[i
				+ 1] != '\"' && input[i + 1] != '\0')
		{
			i++;
			name = get_var_name(input, &i);
			if (!name)
				return (INT_MIN);
			*length += ft_strlen(get_var_value(var, name));
			free(name);
		}
		else
			check_for_spaces(input, length, &i, is_in_quotes);
	}
	return (*length);
}
