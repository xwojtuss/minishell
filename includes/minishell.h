/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkaleta <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 20:34:21 by bkaleta           #+#    #+#             */
/*   Updated: 2024/08/17 12:31:14 by bkaleta          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# define EXIT_SIGINT 130

# define RED_COLOR "\033[0;31m"
# define GREEN_COLOR "\033[0;32m"
# define YELLOW_COLOR "\033[0;33m"
# define BLUE_COLOR "\033[0;34m"
# define MAGENTA_COLOR "\033[0;35m"
# define DEFAULT_COLOR "\033[0m"
# define BOLD_TEXT "\033[1m"
# define NORMAL_TEXT "\033[0m"

# define NOT_SET -42

# define MODE_WRITE 16
# define MODE_APPEND 24

# define FD_PIPE -21

# include "libft.h"
# include <curses.h>
# include <dirent.h>
# include <errno.h>
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
typedef struct s_var
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
	t_cmd			*cmd;
	t_var			*var;
	char			*last_command;
}					t_shell;

extern int			g_signum;

// srcs/alloc.c

t_cmd				*alloc_cmd(int *i, int *command, char *read_path);
// static void	check_for_spaces(char *input, int *length, int *i,
// 		bool is_in_quotes);

int					count_length(char *input, t_var *var);
int					replace_var_loop(char *input, t_var *var, char *result);
char				*replace_var(char *input, t_var *var);
char				**create_array(char *input, t_var *var);

// srcs/check_args.c

bool				is_empty_or_pipe(char *str);
bool				check_redirects(t_shell *shell, char **array);

// srcs/commands.c

t_cmd				*get_last_cmd(t_cmd *cmd);
size_t				count_cmds(t_cmd *cmd);

// srcs/debug.c

void				print_array(char **array);
void				print_cmd(t_cmd *cmd);

// srcs/env_vars.c

void				add_env_var(t_var **var, char *name, char *value);
t_var				*get_var_struct(t_var *var, char *name);
size_t				count_env_vars(t_var *var);
char				*get_var_value(t_var *var, char *name);
char				*get_var_name(char *input, int *i);

// srcs/errors.c

void				print_error(t_shell *shell, char *command, char *message,
						int status);
void				throw_error_exit(char *input, char **array, t_cmd *cmd,
						t_var *var);

// srcs/execute.c

int					execute_builtin(t_cmd *cmd, t_shell *shell, bool do_exit);
char				**get_env_loop(char **env, t_var *tmp);
char				**get_env(t_shell *shell);
int					execute_external(t_cmd *cmd, t_shell *shell);
int					open_redirs(t_cmd *cmd);
int					run_one_builtin(t_cmd *cmd, t_shell *shell);
int					execute(t_shell *shell);

// srcs/fd_management.c

int					get_prev_write_fd(t_cmd *cmd, t_cmd *curr);
int					get_next_read_fd(t_cmd *curr);
void				duplicate_fd(int fd, int std_fd);
void				duplicate_read_fd(t_cmd *cmd, int prev_read_fd);
void				duplicate_write_fd(t_cmd *cmd, int pipe_fd[2]);

// srcs/files.c

char				*locate_file(char *command, char *path);
bool				check_path_for_file(char *path, char *command);
char				*get_path_of_file(char **dirs, char *command);
int					check_file(char *path);
bool				does_file_exist(char *path);

// srcs/free.c

void				close_files(t_cmd *cmd);
void				safely_exit(int exit_code, t_shell *shell, char **array,
						char *input);
void				free_array(char **array);
void				free_var(t_var *var);
void				free_cmd(t_cmd *cmd);

// srcs/ft_cd.c

int					ft_cd(int argc, char **argv, t_shell *shell);

// srcs/ft_echo.c

int					ft_echo(int argc, char **argv);

// srcs/ft_env.c

int					ft_env(int argc, char **argv, t_shell *shell);

// srcs/ft_exit.c

int					ft_exit(int argc, char **argv, t_shell *shell);

// srcs/ft_export.c

int					ft_export(int argc, char **argv, t_shell *shell);

// srcs/ft_pwd.c

int					ft_pwd(int argc, char **argv);

// srcs/ft_unset.c

int					ft_unset(int argc, char **argv, t_shell *shell);

// srcs/init.c

int					redirect(char *str, char *file, t_cmd *cmd, t_shell *shell);
t_cmd				*create_new_cmd_echo(void);
int					read_stdin(t_cmd *new, char *delim);
t_cmd				*read_stdin_delim(char *delim);
int					set_redirect(char *str, t_cmd **cmd, char *file);
char				*copy_without_quotes(char *s);
int					assign_argv(char **array, t_cmd **new, int command);
int					init_cmd(char **array, t_shell *shell);
int					init_env(char **envp, t_shell *shell);

// srcs/input.c

void				set_last_exit_code(t_var *start, int exit_code);
bool				is_in_quotes(char *input, int index);
int					check_empty_pipes(char *input, t_shell *shell);
char				**read_stdin_pipe(char **input, char *temp);
char				*check_pipes(char **input);
int					prelimenary_checks(char **input, t_shell *shell);
void				handle_input(char *input, t_shell *shell);
char				*get_relative_path(char *cwd, t_var *var);
void				set_name_and_hostname(char **prompt_array, t_var *var);
void				set_path_prompt(char *cwd, char **prompt_array, t_var *var);
char				*construct_prompt(char *cwd, t_var *var);
void				wait_for_input(char **envp);

// srcs/main.c

void				child_process_in_main(char **envp);
void				parent_process_in_main(pid_t pid, int status);
int					main(int argc, char **argv, char **envp);

// srcs/misc.c

bool				is_builtin(char *command);
bool				is_a_number(char *s);

// srcs/open_files.c

int					open_temp_file_two(t_cmd *cmd);
int					open_temp_file(t_cmd *cmd);
int					open_normal_file(t_cmd *cmd);

// srcs/path.c

char				*get_absolute_path(char *path);

// srcs/pipe.c

// srcs/redir.c

bool				is_redirect(char *str);
int					redir_input(char *file, t_cmd *cmd, t_shell *shell);
int					redir_output(char *file, t_cmd *cmd, t_shell *shell);
int					redir_append(char *file, t_cmd *cmd, t_shell *shell);
int					redir_delimiter(char *delim, t_cmd *cmd, t_shell *shell);

// srcs/run_processes.c

void				child_routine(t_shell *shell, t_cmd *cmd, int pipe_fd[2],
						int prev_read_fd);
void				parent_routine(t_cmd *cmd, int pipe_fd[2],
						int *prev_read_fd);
pid_t				create_child(t_shell *shell, t_cmd *cmd);
void				wait_for_processes(t_shell *shell, pid_t last_pid);

// srcs/signals.c

void				sig_handler(int signum);
void				sig_do_nothing(int num);
void				handle_eof(t_shell *shell);
void				init_signals(void);
void				signal_quit(int sig);
void				sig_handler(int sig);
void				init_child_signals(void);
void    sig_kill(int signum);
// srcs/split.c

int					add_string(char ****array, int *size, const char *str);
int					copy_word(char *buffer, int *buf_index, char ****result,
						int *size);
char				**split_args(const char *str);

#endif
