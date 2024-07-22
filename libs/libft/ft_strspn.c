#include "libft.h"

size_t	ft_strspn(const char *str1, const char *str2)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (str1[i])
	{
		j = 0;
		while (str2[j])
		{
			if (str1[i] == str2[j])
				break ;
			j++;
		}
		if (!str2[j])
			return (i);
		i++;
	}
	return (i);
}
