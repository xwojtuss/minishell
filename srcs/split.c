#include "minishell.h"

static char	*check_count_quotes(int *quote, char *s, int *count)
{
	while (*s && *s != ' ' && *s != '\'' && *s != '\"')
		s++;
	if (*s == ' ' && *quote == NOT_SET)
	{
		(*count)++;
		while (*s == ' ')
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
	int	count;
	int	quote;

	while (*s == ' ')
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

int	add_string(char ****array, int *size, const char *str)
{
	(**array)[*size] = ft_strdup(str);
	if (!(**array)[*size])
		return (0);
	(*size)++;
	return (1);
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

int	copy_word(char *buffer, int *buf_index, char ****result, int *size)
{
	buffer[*buf_index] = '\0';
	if (!add_string(result, size, buffer))
		return (0);
	*buf_index = 0;
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
		check_quotes(&wait_for, *str);
		if (*str == ' ' && wait_for == NOT_SET && buf_index > 0)
		{
			if (!copy_word(buffer, &buf_index, &result, &size))
				return (0);
		}
		else if (*str && !(*str == ' ' && wait_for == NOT_SET))
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
