#include "libft.h"

int	ft_iswhite(char c)
{
	if ((c >= 0 && c <= 32) || c == 127)
		return (1);
	return (0);
}
