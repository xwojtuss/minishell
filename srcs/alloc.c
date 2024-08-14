#include "minishell.h"

char	*get_var_name(char *input, int *i)
{
	char	*name;

	if (!input || !i)
		return (NULL);
	if (input[*i] == '?')
	{
		(*i)++;
		return (ft_strdup("?"));
	}
	name = ft_strndup(input + *i, ft_strchrs_mf(input + *i, " \"\'") - (input
				+ *i));
	if (!name)
		return (NULL);
	*i += ft_strlen(name);
	return (name);
}

/*
This returns the value of a variable given its name
the return value is not a duplicate of the value, so it should not be freed
*/
char	*get_var_value(t_var *var, char *name)
{
	t_var	*tmp;

	if (!name || !var)
		return (NULL);
	tmp = var;
	while (tmp)
	{
		if (!tmp->name)
			return (NULL);
		if (!ft_strcmp(tmp->name, name))
			return (tmp->value);
		tmp = tmp->next;
	}
	return (NULL);
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
