/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_relative_path.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkornato <wkornato@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 22:08:08 by bkaleta           #+#    #+#             */
/*   Updated: 2024/08/27 11:14:05 by wkornato         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_relative_path(char *cwd, t_var *var)
{
	char	*home;
	char	*temp;

	home = get_var_value(var, "HOME");
	if (!home)
		return (NULL);
	if (!ft_strncmp(cwd, home, ft_strlen(home)))
	{
		temp = ft_strjoin("~", cwd + ft_strlen(home));
		if (!temp)
			return (NULL);
		return (temp);
	}
	else
		return (cwd);
	return (cwd);
}
