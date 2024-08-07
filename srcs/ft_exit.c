#include "minishell.h"

int ft_exit(int argc, char **argv)
{
    int exit_code = 0;
    int i = 0;

	if (argc > 2)
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
    if (argc == 2)
    {
        while (argv[1][i])
        {
            if (!ft_isdigit(argv[1][i]))
            {
				ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
				ft_putstr_fd(argv[1], STDERR_FILENO);
				ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
                exit(2);
            }
            i++;
        }
        exit_code = ft_atoi(argv[1]);
    }
    exit(exit_code);
}