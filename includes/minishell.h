#ifndef MINISHELL_H
# define MINISHELL_H
# define EXIT_SIGINT 130

# define NOT_SET -42

# define FD_PIPE -21

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

/*
Holds the opened files, the path needs to be absolute
*/
typedef struct s_files
{
	char	*path;
	int		fd;
	struct s_files	*next;
}				t_files;

/*
A linked list of commands to execute
*/
typedef struct s_cmd
{
	char			*cmd;// to delete, it has to be argv
	char			**args;
	int				read_fd;
	int				write_fd;
	struct s_cmd	*next;
}					t_cmd;

/*
Stores the name and value of an environment variable
*/
typedef struct s_var//to delete, it has to be a array of strings
{
	char			*name;
	char			*value;
	struct s_var	*next;
}					t_var;

/*
Stores the environment variables and the pointer to the command linked list
will be used in export, unset, env,
	but also to replace the variable with its value in the command
*/
typedef struct s_shell
{
	t_files			*files;
	t_cmd			*cmd;
	t_var			*var;
}					t_shell;

extern int			g_signum;

void				sig_handler(int num);
void				handle_input(char *input, t_shell *shell);
int					pipe_exec(char **cmd1, char **cmd2);
char				**create_array(char *input, t_var *var);
void				free_cmd(t_cmd *cmd);
void				free_array(char **array);
void				throw_error_exit(char *input, char **array, t_cmd *cmd,
						t_var *var);
bool				check_redirects(char **array);
void				wait_for_input(char **envp);
void				env(t_var *var);
void				export(t_var *var, t_cmd *cmd);
void				unset(t_var *var, t_cmd *cmd);
void				print_variables(t_var *var);
int					init_env(char **envp, t_shell *shell);
int					init_cmd(char **array, t_shell *shell);
void				free_var(t_var *var);
// void				sigint_exit(int num);

// TO DELETE:
void				print_array(char **array);

#endif
