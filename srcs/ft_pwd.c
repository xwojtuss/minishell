#include "minishell.h"

int	ft_pwd(int argc, char **argv)
{
	char	*curr_work_dir;

	(void)argc;
	(void)argv;
	curr_work_dir = getcwd(NULL, 0);
	if (!curr_work_dir)
	{
		ft_putstr_fd("minishell: pwd: ", STDERR_FILENO);
		ft_putstr_fd(strerror(errno), STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	free(curr_work_dir);
	return (EXIT_SUCCESS);
}
