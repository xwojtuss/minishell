#include "minishell.h"

/*
Turns the input into an array of strings using ft_split
*/
char	**create_array(char *input)
{
	char	**array;

	array = ft_split(input, ' ');
	return (array);
}