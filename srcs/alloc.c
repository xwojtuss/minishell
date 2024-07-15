#include "minishell.h"

char	**create_array(char *input)
{
	char	**array;

	array = ft_split(input, ' ');
	return (array);
}