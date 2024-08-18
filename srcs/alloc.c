/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkaleta <bkaleta@student.42warsaw.pl>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 12:23:03 by bkaleta           #+#    #+#             */
/*   Updated: 2024/08/18 12:23:05 by bkaleta          ###   ########.fr       */
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

int	count_length(char *input, t_var *var)
{
	int		length;
	bool	is_in_quotes;
	char	*name;
	int		i;

	length = 0;
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
			length += ft_strlen(get_var_value(var, name));
			free(name);
		}
		else
			check_for_spaces(input, &length, &i, is_in_quotes);
	}
	return (length);
}

int	replace_var_loop(char *input, t_var *var, char *result)
{
	int		i;
	int		j;
	bool	is_in_quotes;
	bool	is_in_db_quotes;
	char	*name;

	i = 0;
	j = 0;
	name = NULL;
	is_in_quotes = false;
	is_in_db_quotes = false;
	while (input[i])
	{
		if (input[i] == '\'')
			is_in_quotes = !is_in_quotes;
		else if (input[i] == '\"')
			is_in_db_quotes = !is_in_db_quotes;
		if (input[i] == '$' && !is_in_quotes && input[i + 1] != ' ' && input[i
			+ 1] != '\"' && input[i + 1] != '\0')
		{
			i++;
			name = get_var_name(input, &i);
			if (!name)
				return (0);
			ft_strlcpy(result + j, get_var_value(var, name),
				ft_strlen(get_var_value(var, name)) + 1);
			j += ft_strlen(get_var_value(var, name));
			free(name);
		}
		else
		{
			if (i > 0 && input[i] == '|' && input[i - 1] != ' ' && !is_in_quotes
				&& !is_in_db_quotes)
				result[j++] = ' ';
			else if (i > 0 && input[i] == '<' && input[i - 1] != ' ' && input[i
				- 1] != '<' && !is_in_quotes && !is_in_db_quotes)
				result[j++] = ' ';
			else if (i > 0 && input[i] == '>' && input[i - 1] != ' ' && input[i
				- 1] != '>' && !is_in_quotes && !is_in_db_quotes)
				result[j++] = ' ';
			result[j] = input[i];
			if (input[i] == '|' && input[i + 1] != ' ' && input[i + 1] != '\0'
				&& !is_in_quotes && !is_in_db_quotes)
				result[++j] = ' ';
			else if (input[i] == '<' && input[i + 1] != ' ' && input[i
				+ 1] != '<' && input[i + 1] != '\0' && !is_in_quotes
				&& !is_in_db_quotes)
				result[++j] = ' ';
			else if (input[i] == '>' && input[i + 1] != ' ' && input[i
				+ 1] != '>' && input[i + 1] != '\0' && !is_in_quotes
				&& !is_in_db_quotes)
				result[++j] = ' ';
			j++;
			i++;
		}
	}
	return (1);
}

char	*replace_var(char *input, t_var *var)
{
	int		length;
	char	*result;

	length = count_length(input, var);
	if (length <= 0)
		return (NULL);
	result = (char *)ft_calloc(length + 1, sizeof(char));
	if (!result)
		return (NULL);
	if (!replace_var_loop(input, var, result) || !result)
		return (free(result), NULL);
	return (result);
}

/*
Turns the input into an array of strings using ft_split
*/
char	**create_array(char *input, t_var *var)
{
	char	*input_replaced;
	char	**result;

	input_replaced = replace_var(input, var);
	if (!input_replaced)
		return (NULL);
	result = split_args(input_replaced);
	free(input_replaced);
	return (result);
}
