/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_array_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkornato <wkornato@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/25 15:20:02 by wkornato          #+#    #+#             */
/*   Updated: 2024/08/25 16:24:35 by wkornato         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	do_copy_var(int quotes, char *input, int i)
{
	return ((input[i] == '$' && quotes != '\'' && input[i + 1] != ' '
			&& input[i + 1] != '\"' && input[i + 1] != '\0'));
}

int	copy_variable(char *name, t_var *var, int *j, char *result)
{
	if (!name)
		return (0);
	ft_strlcpy(result + *j, get_var_value(var, name),
		ft_strlen(get_var_value(var, name)) + 1);
	*j += ft_strlen(get_var_value(var, name));
	free(name);
	return (1);
}

char	*replace_var(char *input, t_var *var)
{
	int		length;
	char	*result;

	length = 0;
	count_length(input, var, &length);
	if (length <= 0)
		return (NULL);
	result = (char *)ft_calloc(length + 1, sizeof(char));
	if (!result)
		return (NULL);
	if (!replace_var_loop(input, var, result, 0) || !result)
		return (free(result), NULL);
	return (result);
}
