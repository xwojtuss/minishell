/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkaleta <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 13:51:07 by wkornato          #+#    #+#             */
/*   Updated: 2024/08/16 22:10:43 by bkaleta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_in_quotes(char *input, int index)
{
	int	quote;
	int	i;

	i = 0;
	quote = NOT_SET;
	while (i < index)
	{
		if (input[i] == '\'' && quote == NOT_SET)
			quote = '\'';
		else if (input[i] == '\"' && quote == NOT_SET)
			quote = '\"';
		else if (input[i] == quote)
			quote = NOT_SET;
		i++;
	}
	return (quote != NOT_SET);
}

int	check_empty_pipes(char *input, t_shell *shell)
{
	int	i;
	int	j;

	i = 0;
	while (input[i])
	{
		if (input[i] == '|')
		{
			j = i - 1;
			while (j >= 0 && input[j] && ft_iswhite(input[j]))
				j--;
			if ((j < 0 || input[j] == '|' || input[j] == '<')
				&& !is_in_quotes(input, j))
			{
				ft_putstr_fd("minishell: syntax error near unexpected token `|'\n",
					STDERR_FILENO);
				set_last_exit_code(shell->var, 2);
				add_history((const char *)input);
				return (0);
			}
		}
		i++;
	}
	return (1);
}

char	**read_stdin_pipe(char **input, char *temp)
{
	char	*stdin_line;

	write(STDOUT_FILENO, "> ", 2);
	stdin_line = get_next_line(0);
	if (!stdin_line)
		return (NULL);
	if (ft_strlen(stdin_line) == 0)
	{
		free(stdin_line);
		return (NULL);
	}
	if (stdin_line[ft_strlen(stdin_line) - 1] == '\n')
		stdin_line[ft_strlen(stdin_line) - 1] = '\0';
	*input = ft_strjoin_delim(temp, ' ', stdin_line);
	free(temp);
	free(stdin_line);
	if (!*input)
		return (NULL);
	return (input);
}

char	*check_pipes(char **input)
{
	char	*temp;
	char	*last;

	while (true)
	{
		temp = *input;
		last = ft_strrchr(*input, '|');
		if (!last)
			break ;
		last++;
		while (*last && ft_iswhite(*last))
			last++;
		if (*last)
			break ;
		else
			input = read_stdin_pipe(input, temp);
	}
	return (*input);
}
