#include "minishell.h"

static int	count_words(char const *s)
{
	int		i;
	int		count;
	bool	is_in_quotes;

	i = 0;
	count = 0;
	is_in_quotes = false;
	while (s[i])
	{
		if (s[i] == '\"' || s[i] == '\'')
			is_in_quotes = !is_in_quotes;
		if (s[i] != ' ' || is_in_quotes)
		{
			count++;
			while (s[i] && s[i] != ' ')
			{
				if (s[i] == '\"' || s[i] == '\'')
				{
					i++;
					while (s[i] && s[i] != '\"' && s[i] != '\'')
						i++;
				}
				i++;
			}
		}
		while (s[i] && s[i] == ' ')
			i++;
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

static int	copy_data(const char *str, char ***result, char *buffer, int buf_index)
{
	bool	in_quotes;
	int		size;

	size = 0;
	in_quotes = false;
	while (*str)
	{
		if (*str == '"')
			in_quotes = !in_quotes;
		else if (*str == ' ' && !in_quotes && buf_index > 0)
		{
			buffer[buf_index] = '\0';
			if (!add_string(&result, &size, buffer))
				return (0);
			buf_index = 0;
		}
		else if (!(*str == ' ' && !in_quotes))
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
	char **result;
	char *buffer;
	int buf_index;

	result = NULL;
	buffer = NULL;
	buf_index = 0;
	buffer = (char *)ft_calloc(strlen(str) + 1, sizeof(char));
	if (!buffer)
		return (0);
	result = (char **)ft_calloc(count_words(str) + 1, sizeof(char *));
	if (!result)
		return (free(buffer), NULL);
	result[count_words(str)] = NULL;
	if (!copy_data(str, &result, buffer, buf_index))
		return (free(buffer), free_array(result), NULL);
	free(buffer);
	return (result);
}