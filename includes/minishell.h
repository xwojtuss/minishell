#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <curses.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <term.h>
# include <unistd.h>

typedef struct s_cmd
{
	char			*cmd;
	char			**args;
	int				read_fd;
	int				write_fd;
	// bonus-int for checking the previous exit status
	struct s_cmd	*next;
}					t_cmd;

typedef struct s_shell
{
	t_cmd			*cmd;
	char			**env;
}					t_shell;

extern int			g_signum;

void				sig_handler(int num);
void				handle_input(char *input);
int					pipe_exec(char **cmd1, char **cmd2);
char				**create_array(char *input);
void				free_cmd(t_cmd *cmd);
void				free_array(char **array);
void				throw_error_exit(char *input, char **array, t_cmd *cmd);
bool				check_redirects(char **array);
void				wait_for_input(void);

// TO DELETE:
void				print_array(char **array);

#endif
