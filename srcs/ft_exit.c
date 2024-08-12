#include "minishell.h"

int ft_exit(int argc, char **argv, t_shell *shell)
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
        {
            if (!ft_isdigit(argv[1][i]))
            {
				if (argv[1][i] == '-')
                    safely_exit(156, shell, NULL, NULL);
                else if (argv[1][i] == '+')
                    safely_exit(100, shell, NULL, NULL);
                else
                    safely_exit(2, shell, NULL, NULL);
            }
            i++;
        }
        exit_code = ft_atoi(argv[1]);
    }
    safely_exit(exit_code, shell, NULL, NULL);
    return (exit_code);
}