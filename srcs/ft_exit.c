#include "minishell.h"

static void	check_arg(t_shell *shell, char *arg, int i)
{
	if (!ft_isdigit(arg[i]))
	{
		if (arg[i] == '-')
			safely_exit(156, shell, NULL, NULL);
		else if (arg[i] == '+')
			safely_exit(100, shell, NULL, NULL);
		else
		{
			ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
			ft_putstr_fd(arg, STDERR_FILENO);
			ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
			safely_exit(2, shell, NULL, NULL);
		}
	}
}

int	ft_exit(int argc, char **argv, t_shell *shell)
{
	int exit_code = 0;
	int i = 0;

	if (argc > 2)
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
		safely_exit(EXIT_FAILURE, shell, NULL, NULL);
	}
	if (argc == 2)
	{
		while (argv[1][i])
			check_arg(shell, argv[1], i++);
		exit_code = ft_atoi(argv[1]);
	}
	safely_exit(exit_code, shell, NULL, NULL);
	return (exit_code);
}
