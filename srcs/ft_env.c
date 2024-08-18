/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkaleta <bkaleta@student.42warsaw.pl>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 12:27:32 by bkaleta           #+#    #+#             */
/*   Updated: 2024/08/18 12:27:33 by bkaleta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
Prints the environment variables
*/
int	ft_env(int argc, char **argv, t_shell *shell)
{
	t_var	*curr;

	(void)argc;
	(void)argv;
	curr = shell->var->next;
	while (curr)
	{
		if (curr->name && curr->value)
			printf("%s=%s\n", curr->name, curr->value);
		curr = curr->next;
	}
	return (EXIT_SUCCESS);
}
