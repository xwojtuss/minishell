/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_array.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkornato <wkornato@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/25 13:50:02 by wkornato          #+#    #+#             */
/*   Updated: 2024/08/25 16:24:32 by wkornato         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	add_another_space(int i, int quotes, char *input)
{
	if (input[i] == '|' && input[i + 1] != ' ' && input[i + 1] != '\0'
		&& quotes == NOT_SET)
		return (1);
	else if (((input[i] == '<' && input[i + 1] != '<') || (input[i] == '>'
				&& input[i + 1] != '>')) && input[i + 1] != ' ' && input[i
			+ 1] != '\0' && quotes == NOT_SET)
		return (1);
	return (0);
}

static int	add_space(int i, int quotes, char *input)
{
	if (i > 0 && input[i] == '|' && input[i - 1] != ' ' && quotes == NOT_SET)
		return (1);
	else if (i > 0 && ((input[i] == '>' && input[i - 1] != '>')
			|| (input[i] == '<' && input[i - 1] != '<')) && input[i - 1] != ' '
		&& quotes == NOT_SET)
		return (1);
	return (0);
}

static void	set_quotes(char *quotes, char word)
{
	if (*quotes == NOT_SET && (word == '\'' || word == '\"'))
		*quotes = word;
	else if (*quotes != NOT_SET && word == *quotes)
		*quotes = NOT_SET;
}

int	replace_var_loop(char *input, t_var *var, char *result, int i)
{
	int		j;
	char	quotes;

	j = 0;
	quotes = NOT_SET;
	while (input[i])
	{
		set_quotes(&quotes, input[i]);
		if (!do_copy_var(quotes, input, i))
		{
			if (add_space(i, quotes, input))
				result[j++] = ' ';
			result[j] = input[i];
			if (add_another_space(i, quotes, input))
				result[++j] = ' ';
			i++;
			j++;
		}
		else
		{
			i++;
			copy_variable(get_var_name(input, &i), var, &j, result);
		}
	}
	return (1);
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
