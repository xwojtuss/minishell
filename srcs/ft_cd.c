/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkornato <wkornato@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 23:46:58 by wkornato          #+#    #+#             */
/*   Updated: 2024/08/15 00:30:17 by wkornato         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	modify_pwd(t_shell *shell)
{
	t_var	*oldpwd;

	oldpwd = get_var_struct(shell->var, "PWD");
	if (shell && shell->var && oldpwd)
	{
		if (get_var_struct(shell->var, "OLDPWD") != NULL)
		{
			if (get_var_struct(shell->var, "OLDPWD")->value != NULL)
				free(get_var_struct(shell->var, "OLDPWD")->value);
			get_var_struct(shell->var,
					"OLDPWD")->value = ft_strdup(oldpwd->value);
		}
		else
			add_env_var(&shell->var, "OLDPWD", oldpwd->value);
		free(oldpwd->value);
		if (get_var_struct(shell->var, "PWD") != NULL)
			get_var_struct(shell->var, "PWD")->value = getcwd(NULL, 0);
	}
}

int	ft_cd(int argc, char **argv, t_shell *shell)
{
	char	*path;

	if (argc == 1)
		return (ft_putstr_fd("minishell: cd: not enough arguments\n",
				STDERR_FILENO), EXIT_FAILURE);
	if (argc != 2)
		return (ft_putstr_fd("minishell: cd: too many arguments\n",
				STDERR_FILENO), EXIT_FAILURE);
	path = get_absolute_path(argv[1]);
	if (chdir(path) != 0)
	{
		free(path);
		ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
		ft_putstr_fd(argv[1], STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		ft_putstr_fd(strerror(errno), STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	modify_pwd(shell);
	free(path);
	return (EXIT_SUCCESS);
}
