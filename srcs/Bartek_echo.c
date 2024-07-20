#include "minishell.h"

void	Bartek_echo(t_cmd *cmd)
{
	int	i;
	int	j;
	int new_line;

	i = 1;
	new_line = 1;

	while (cmd->args[1] && ft_strncmp(cmd->args[i], "-n", 2) == 0)
	{
		j = 2;
		while (cmd->args[i][j] == 'n')
			j++;
		if (cmd->args[i][j] == '\0')
		{
			new_line = 0;
			i++;
		}
		else
			break ;
	}
	while (cmd->args[i])
	{
		write(cmd->write_fd, cmd->args[i], ft_strlen(cmd->args[i]));
		if (cmd->args[i + 1])
			write(cmd->write_fd, " ", 1);
		i++;
	}
	if (new_line)
		write(cmd->write_fd, "\n", 1);
}