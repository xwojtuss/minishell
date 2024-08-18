/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkaleta <bkaleta@student.42warsaw.pl>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 12:36:15 by bkaleta           #+#    #+#             */
/*   Updated: 2024/08/18 12:36:35 by bkaleta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_name_and_hostname(char **prompt_array, t_var *var)
{
	prompt_array[0] = GREEN_COLOR;
	prompt_array[1] = BOLD_TEXT;
	prompt_array[2] = ft_fallback_string(get_var_value(var, "USER"), "user");
	prompt_array[3] = "@";
	prompt_array[4] = ft_fallback_string(get_var_value(var, "NAME"),
			"hostname");
	prompt_array[5] = DEFAULT_COLOR;
	prompt_array[6] = NORMAL_TEXT;
}

void	set_path_prompt(char *cwd, char **prompt_array, t_var *var)
{
	prompt_array[7] = ":";
	prompt_array[8] = BLUE_COLOR;
	prompt_array[9] = BOLD_TEXT;
	prompt_array[10] = ft_fallback_string(get_relative_path(cwd, var),
			cwd);
	prompt_array[11] = DEFAULT_COLOR;
	prompt_array[12] = NORMAL_TEXT;
	prompt_array[13] = "$ ";
}

char	*construct_prompt(char *cwd, t_var *var)
// only a preview,there is a way more efficient way to do this
{
	char	**prompt_array;
	char	*prompt;

	prompt_array = (char **)ft_calloc(15, sizeof(char *));
	if (!prompt_array)
		return (NULL);
	set_name_and_hostname(prompt_array, var);
	set_path_prompt(cwd, prompt_array, var);
	prompt = ft_strjoin_array(prompt_array);
	if (ft_strcmp(prompt_array[10], cwd) != 0)
		free(prompt_array[10]);
	(void)cwd;
	free(prompt_array);
	return (prompt);
}
