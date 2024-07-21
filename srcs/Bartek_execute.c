#include "minishell.h"

void	Bartek_execute(t_cmd *cmd)
{
	if (NULL == cmd || NULL == cmd->args || NULL == cmd->args[0])
		return ;
	
	if (ft_strcmp(cmd->args[0], "echo") == 0)
	{
		Bartek_echo(cmd);
		return ;
	}
	else if (ft_strcmp(cmd->args[0], "pwd") == 0)
	{
		Bartek_pwd(cmd);
		return ;
	}
}