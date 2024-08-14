#include "minishell.h"

void	Bartek_exit(t_cmd *cmd)
{
	int exit_code = 0;
	int i = 0;

	if (cmd->args[1])
	{
		while (cmd->args[1][i])
		{
			if (!ft_isdigit(cmd->args[1][i]))
			{
				write(cmd->write_fd, "minishell: exit: ", 18);
				write(cmd->write_fd, cmd->args[1], ft_strlen(cmd->args[1]));
				write(cmd->write_fd, ": numeric argument required\n", 29);
				exit(2);
			}
			i++;
		}
		exit_code = ft_atoi(cmd->args[1]);

		if (cmd->args[2])
		{
			write(cmd->write_fd, "minishell: exit: too many arguments\n", 37);
			return ;
		}
	}
	exit(exit_code);
}
