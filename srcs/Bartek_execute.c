#include "minishell.h"

void	Bartek_execute(t_cmd *cmd, t_shell *shell)
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
	else if (ft_strcmp(cmd->args[0], "cd") == 0)
	{
		Bartek_cd(cmd);
		return ;
	}
	else if (ft_strcmp(cmd->args[0], "exit") == 0)
	{
		Bartek_exit(cmd);
	}
	else if (ft_strcmp(cmd->args[0], "unset") == 0)
	{
		Bartek_unset(cmd, shell);
		return ;
	}
	else if (ft_strcmp(cmd->args[0], "export") == 0)
	{
		// tutaj bedzie funkcja
		return ;
	}
}