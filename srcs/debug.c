#include "minishell.h"

/*
Prints the array of strings, the array must be NULL terminated
*/
void	print_array(char **array)
{
	int i;

	i = 0;
	while (array[i])
	{
		printf("array[%d]: %s\n", i, array[i]);
		i++;
	}
}
