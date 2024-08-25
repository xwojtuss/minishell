/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkornato <wkornato@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/25 13:49:29 by wkornato          #+#    #+#             */
/*   Updated: 2024/08/25 13:49:34 by wkornato         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**get_env_loop(char **env, t_var *tmp)
{
	int		i;
	char	*temp;

	i = 0;
	while (tmp)
	{
		env[i] = ft_strjoin(tmp->name, "=");
		if (!env[i])
			return (NULL);
		if (tmp->value && tmp->value[0] != '\0')
		{
			temp = env[i];
			env[i] = ft_strjoin(temp, tmp->value);
			free(temp);
			if (!env[i])
				return (NULL);
		}
		tmp = tmp->next;
		i++;
	}
	return (env);
}

char	**get_env(t_shell *shell)
{
	char	**env;
	t_var	*tmp;

	tmp = shell->var;
	env = (char **)ft_calloc(count_env_vars(shell->var) + 1, sizeof(char *));
	if (!env)
		return (NULL);
	env = get_env_loop(env, tmp);
	return (env);
}
