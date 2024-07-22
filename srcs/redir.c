#include "minishell.h"

bool	is_redirect(char *str)
{
	if (!ft_strcmp(str, ">") || !ft_strcmp(str, ">>") || !ft_strcmp(str, "<<") || !ft_strcmp(str, "<"))
		return (true);
	return (false);
}

int	redir_input(int fd, t_cmd *cmd, t_shell *shell)
{
	(void)fd;
	(void)cmd;
	(void)shell;
	return (1);
}

int	redir_output(int fd, t_cmd *cmd, t_shell *shell)
{
	(void)fd;
	(void)cmd;
	(void)shell;
	return (1);
}

int	redir_append(int fd, t_cmd *cmd, t_shell *shell)
{
	(void)fd;
	(void)cmd;
	(void)shell;
	return (1);
}

int	redir_delimiter(int fd, t_cmd *cmd, t_shell *shell)
{
	(void)fd;
	(void)cmd;
	(void)shell;
	return (1);
}