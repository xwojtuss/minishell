/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_vars.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkaleta <bkaleta@student.42warsaw.pl>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 12:24:46 by bkaleta           #+#    #+#             */
/*   Updated: 2024/08/18 12:24:57 by bkaleta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_env_var(t_var **var, char *name, char *value)
{
	t_var	*new;
	t_var	*tmp;

	new = (t_var *)malloc(sizeof(t_var));
	if (!new)
		return ;
	new->name = ft_strdup(name);
	if (!new->name)
		return (free(new));
	new->value = ft_strdup(value);
	if (!new->value && value)
		return (free(new->name), free(new));
	new->next = NULL;
	tmp = *var;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

t_var	*get_var_struct(t_var *var, char *name)
{
	t_var	*tmp;

	tmp = var;
	while (tmp)
	{
		if (!ft_strcmp(tmp->name, name))
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}

size_t	count_env_vars(t_var *var)
{
	size_t	count;
	t_var	*tmp;

	count = 0;
	tmp = var;
	while (tmp)
	{
		count++;
		tmp = tmp->next;
	}
	return (count);
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
