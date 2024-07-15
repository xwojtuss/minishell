#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

typedef struct s_cmd
{
	char			*cmd;
	char			**args;
	int				read_fd;
	int				write_fd;
	//bonus-int for checking the previous exit status
	struct s_cmd	*next;
}					t_cmd;

extern int			g_signum;

void				sig_handler(int num);
void				handle_input(char *input);
int					pipe_exec(char **cmd1, char **cmd2);
char	**create_array(char *input);
void	free_cmd(t_cmd *cmd);
void	free_array(char **array);
void	throw_error(char *input, char **array, t_cmd *cmd);

//TO DELETE:
void	print_array(char **array);

#endif
