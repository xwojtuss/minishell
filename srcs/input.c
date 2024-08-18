/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkaleta <bkaleta@student.42warsaw.pl>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 12:34:33 by bkaleta           #+#    #+#             */
/*   Updated: 2024/08/18 12:34:37 by bkaleta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_last_exit_code(t_var *start, int exit_code)
{
	t_var	*tmp;

	if (!start || exit_code < 0)
		return ;
	tmp = get_var_struct(start, "?");
	if (!tmp)
		return ;
	if (tmp->value)
		free(tmp->value);
	tmp->value = ft_itoa(exit_code);
	if (!tmp->value)
		return ;
}

int	prelimenary_checks(char **input, t_shell *shell)
{
	shell->cmd = NULL;
	if (*input == NULL)
	{
		rl_clear_history();
		free_var(shell->var);
		exit(EXIT_SUCCESS);
	}
	if (!**input)
		return (free(*input), 0);
	if (!check_empty_pipes(*input, shell))
		return (free(*input), 0);
	*input = check_pipes(input);
	if (!*input)
		throw_error_exit(NULL, NULL, NULL, shell->var);
	add_history((const char *)*input);
	return (1);
}

/*
Prepares the input for execution, prelimenary checks are called from here
*/
void	handle_input(char *input, t_shell *shell)
{
	char	**array;

	if (!prelimenary_checks(&input, shell))
		return ;
	array = create_array(input, shell->var);
	free(input);
	if (!array || !check_redirects(shell, array))
		return ;
	if (!init_cmd(array, shell) || !shell->cmd || !shell->cmd->argv
		|| !shell->cmd->argv[0])
		return (free_cmd(shell->cmd), free_array(array));
	free_array(array);
	if (!execute(shell))
		throw_error_exit(NULL, NULL, shell->cmd, shell->var);
	free_cmd(shell->cmd);
}

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
		home = temp;
	}
	return (home);
}

void	wait_for_input(char **envp)
{
	char	*input;
	char	*cwd;
	char	*prompt;
	t_shell	shell;

	if (!init_env(envp, &shell))
		throw_error_exit(NULL, NULL, NULL, shell.var);
	while (1)
	{
		cwd = getcwd(NULL, 0);			
		if (!cwd)
			throw_error_exit(NULL, NULL, NULL, shell.var);
		prompt = construct_prompt(cwd, shell.var);
		free(cwd);
		if (!prompt)
			throw_error_exit(NULL, NULL, NULL, shell.var);
		input = readline(prompt);
		free(prompt);
		if (!input)
			handle_eof(&shell);
		else
			handle_input(input, &shell);
	}
	free(prompt);
	free_var((&shell)->var);
}
