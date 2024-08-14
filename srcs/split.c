#include "minishell.h"

static int	count_words(char const *s)
{
	int	count;
	int	quote;

	while (*s == ' ')
		s++;
	if (*s == '\0')
		return (0);
	count = 1;
	quote = NOT_SET;
	while (*s)
	{
		while (*s && *s != ' ' && *s != '\'' && *s != '\"')
			s++;
		if (*s == ' ' && quote == NOT_SET)
		{
			count++;
			while (*s == ' ')
				s++;
		}
		else if (*s == '\'' || *s == '\"')
		{
			quote = *s;
			s++;
			while (*s && *s != quote)
				s++;
			if (!*s)
				break ;
			s++;
			quote = NOT_SET;
		}
		else if (*s)
			s++;
	}
	return (count);
}

int	add_string(char ****array, int *size, const char *str)
{
	(**array)[*size] = ft_strdup(str);
	if (!(**array)[*size])
		return (0);
	(*size)++;
	return (1);
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
		if (*str == '\"' && wait_for == NOT_SET)
			wait_for = '\"';
		else if (*str == '\'' && wait_for == NOT_SET)
			wait_for = '\'';
		else if (*str == wait_for)
			wait_for = NOT_SET;
		if (*str == ' ' && wait_for == NOT_SET && buf_index > 0)
		{
			buffer[buf_index] = '\0';
			if (!add_string(&result, &size, buffer))
				return (0);
			buf_index = 0;
		}
		else if (*str && !(*str == ' ' && wait_for == NOT_SET))
			buffer[buf_index++] = *str;
		str++;
	}
	if (buf_index > 0)
	{
		buffer[buf_index] = '\0';
		if (!add_string(&result, &size, buffer))
			return (0);
	}
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
