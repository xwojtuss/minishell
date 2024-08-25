/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stdin.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkornato <wkornato@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/25 13:49:19 by wkornato          #+#    #+#             */
/*   Updated: 2024/08/25 13:49:20 by wkornato         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*create_new_cmd_echo(void)
{
	t_cmd	*new;

	new = alloc_cmd(NULL, NULL, NULL);
	if (!new)
		return (NULL);
	new->argv = (char **)ft_calloc(3, sizeof(char *));
	if (!(new->argv))
		return (free_cmd(new), NULL);
	new->argv[0] = ft_strdup("echo");
	if (!new->argv[0])
		return (free_cmd(new), NULL);
	new->argv[1] = ft_strdup("");
	if (!(new->argv[1]))
		return (free_cmd(new), NULL);
	new->argc = 2;
	return (new);
}

int	read_stdin(t_cmd *new, char *delim)
{
	char	*stdin_line;
	char	*temp;

	stdin_line = get_next_line(0);
	if (!stdin_line)
	{
		ft_putstr_fd("\nminishell: warning here-document at ", STDERR_FILENO);
		ft_putstr_fd("this line delimited by end-of-file (wanted '",
			STDERR_FILENO);
		ft_putstr_fd(delim, STDERR_FILENO);
		ft_putstr_fd("')\n", STDERR_FILENO);
		return (0);
	}
	if (ft_strlen(stdin_line) == 0)
		return (free(stdin_line), 1);
	if (!ft_strncmp(stdin_line, delim, ft_max(ft_strlen(delim),
				ft_strlen(stdin_line) - 1)))
		return (free(stdin_line), 2);
	temp = new->argv[1];
	new->argv[1] = ft_strjoin(temp, stdin_line);
	free(temp);
	free(stdin_line);
	if (!(new->argv[1]))
		return (0);
	return (1);
}

t_cmd	*read_stdin_delim(char *delim)
{
	t_cmd	*new;
	int		ret;

	new = create_new_cmd_echo();
	if (!new)
		return (0);
	while (true)
	{
		write(STDOUT_FILENO, "> ", 2);
		ret = read_stdin(new, delim);
		if (ret == 2)
			break ;
		else if (ret == 0)
			return (free_cmd(new), NULL);
	}
	if (new && new->argv && new->argv[1] && ft_strlen(new->argv[1]) > 0)
		new->argv[1][ft_strlen(new->argv[1]) - 1] = '\0';
	new->argv[2] = NULL;
	return (new);
}
