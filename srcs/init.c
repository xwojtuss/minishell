/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkornato <wkornato@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 12:29:59 by bkaleta           #+#    #+#             */
/*   Updated: 2024/08/25 13:48:13 by wkornato         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	assign_curr(t_cmd **curr, t_shell *shell, t_cmd *new,
		char **next_read_path)
{
	if (!*curr)
		shell->cmd = new;
	else
		(*curr)->next = new;
	if (new->next)
		*curr = new->next;
	else
		*curr = new;
	*next_read_path = NULL;
}

void	set_as_failure(t_shell *shell, t_cmd *new, char **next_read_path)
{
	set_last_exit_code(shell->var, EXIT_FAILURE);
	free_cmd(new);
	free_cmd(shell->cmd);
	shell->cmd = NULL;
	*next_read_path = "./.minishell_empty_file";
}

int	init_cmd(char **array, t_shell *shell)
{
	t_cmd	*new;
	t_cmd	*curr;
	char	*next_read_path;
	int		i;
	int		command;

	i = 0;
	curr = NULL;
	next_read_path = NULL;
	while (array[i])
	{
		new = alloc_cmd(&i, &command, next_read_path);
		if (!new)
			return (0);
		if (assign_argv(array, &new, command))
			assign_curr(&curr, shell, new, &next_read_path);
		else
			set_as_failure(shell, new, &next_read_path);
		while (array[i] && ft_strcmp(array[i], "|"))
			i++;
		if (array[i])
			i++;
	}
	return (1);
}

int	copy_variables(char **envp, t_var **curr)
{
	t_var	*new;
	int		i;

	i = 0;
	while (envp[i])
	{
		new = (t_var *)malloc(sizeof(t_var));
		if (!new)
			return (0);
		new->name = ft_strndup(envp[i], ft_strchr(envp[i], '=') - envp[i]);
		if (!new->name)
			return (0);
		new->value = ft_strdup(ft_strchr(envp[i], '=') + 1);
		if (!new->value)
			return (0);
		new->next = NULL;
		(*curr)->next = new;
		*curr = new;
		i++;
	}
	return (1);
}

int	init_env(char **envp, t_shell *shell)
{
	t_var	*curr;

	curr = (t_var *)malloc(sizeof(t_var));
	if (!curr)
		return (0);
	curr->name = ft_strdup("?");
	if (!curr->name)
		return (0);
	curr->value = ft_strdup("0");
	if (!curr->value)
		return (0);
	curr->next = NULL;
	shell->var = curr;
	return (copy_variables(envp, &curr));
}
