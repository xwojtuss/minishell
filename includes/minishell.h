#ifndef MINISHELL_H
# define MINISHELL_H
# define EXIT_SIGINT 130

# define NOT_SET -42

# define MODE_WRITE 16
# define MODE_APPEND 24

# define FD_PIPE -21

# include "libft.h"
# include <curses.h>
# include <fcntl.h>
# include <linux/limits.h>
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
/* typedef struct s_files
{
	char			*path;
	int				fd;
	struct s_files	*next;
}					t_files; */

/*
A linked list of commands to execute
*/
typedef struct s_cmd
{
	int				argc;
	char			**argv;
	int				read_fd;
	int				write_fd;
	int				write_mode;
	char			*read_path;
	char			*write_path;
	struct s_cmd	*next;
}					t_cmd;

/*
Stores the name and value of an environment variable
*/
typedef struct s_var // to delete, it has to be a array of strings
{
	char *name;
	char *value;
	struct s_var *next;
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
char				**split_args(char const *s);
char				*get_absolute_path(char *path);
void				free_files(t_files *files);
int					redirect(char *type, char *path, t_cmd *cmd, t_shell *shell);
bool					is_redirect(char *str);
int					count_length(char *input, t_var *var);
int					replace_var_loop(char *input, t_var *var, char *result);
char				*replace_var(char *input, t_var *var);
void				handle_input(char *input, t_shell *shell);
void				wait_for_input(char **envp);
void				sigint_exit(int num);
int					redir_input(char *file, t_cmd *cmd, t_shell *shell);
int					redir_append(char *file, t_cmd *cmd, t_shell *shell);
int					redir_output(char *file, t_cmd *cmd, t_shell *shell);
int					redir_delimiter(char *delim, t_cmd *cmd, t_shell *shell);
int					get_fd(char *path, t_files *files);
t_files				*add_file(char *path, int fd, t_files *files);

// void				sigint_exit(int num);

// Bartkowe funkcje czyli gowno ale moze jakos dziala
t_cmd				*Bartek_init_cmd(char **array);
void				Bartek_execute(t_cmd *cmd, t_shell *shell);
void				Bartek_echo(t_cmd *cmd);
void				Bartek_handle_error(const char *message_to_print);
void				Bartek_pwd(t_cmd *cmd);
void				Bartek_cd(t_cmd *cmd);
int					is_num(const char *str);
void				Bartek_exit(t_cmd *cmd);
void				Bartek_unset(t_cmd *cmd, t_shell *shell);

// TO DELETE:
void				print_array(char **array);

#endif
