#include "minishell.h"

bool	is_builtin(char *command)
{
	if (!command)
		return (false);
	if (!ft_strcmp(command, "echo") || !ft_strcmp(command, "cd")
		|| !ft_strcmp(command, "pwd") || !ft_strcmp(command, "export")
		|| !ft_strcmp(command, "unset") || !ft_strcmp(command, "env")
		|| !ft_strcmp(command, "exit"))
		return (true);
	return (false);
}

int	execute_builtin(t_cmd *cmd, t_shell *shell, bool do_exit)
{
	int	ret;

	ret = EXIT_FAILURE;
	if (!cmd->argv || !cmd->argv[0])
		ret = EXIT_FAILURE;
	else if (!ft_strcmp(cmd->argv[0], "echo"))
		ret = ft_echo(cmd->argc, cmd->argv);
	else if (!ft_strcmp(cmd->argv[0], "cd"))
		ret = ft_cd(cmd->argc, cmd->argv, shell);
	else if (!ft_strcmp(cmd->argv[0], "pwd"))
		ret = ft_pwd(cmd->argc, cmd->argv);
	else if (!ft_strcmp(cmd->argv[0], "export"))
		ret = ft_export(cmd->argc, cmd->argv, shell);
	else if (!ft_strcmp(cmd->argv[0], "unset"))
		ret = ft_unset(cmd->argc, cmd->argv, shell);
	else if (!ft_strcmp(cmd->argv[0], "env"))
		ret = ft_env(cmd->argc, cmd->argv, shell);
	else if (!ft_strcmp(cmd->argv[0], "exit"))
		ret = ft_exit(cmd->argc, cmd->argv, shell);
	if (do_exit)
		safely_exit(ret, shell, NULL, NULL);
	return (ret);
}

bool	check_path_for_file(char *path, char *command)
{
	DIR				*dir;
	struct dirent	*info;

	dir = opendir(path);
	if (!dir)
		return (false);
	while (true)
	{
		info = readdir(dir);
		if (!info)
			break ;
		if (!ft_strcmp(info->d_name, command))
		{
			closedir(dir);
			return (true);
		}
	}
	closedir(dir);
	return (false);
}

char	*get_path_of_file(char **dirs, char *command)
{
	int	i;

	i = 0;
	while (dirs[i])
	{
		if (check_path_for_file(dirs[i], command))
			break ;
		i++;
	}
	return (dirs[i]);
}

/*
Checks if the file is in the specified path,
there can be multiple paths separated by ':'
if the file is found, it returns the full path
*/
char	*locate_file(char *command, char *path)
{
	char	**dirs;
	char	*result;
	char	*temp;

	if (!path || !command)
		return (NULL);
	dirs = ft_split(path, ':');
	if (!dirs)
		return (NULL);
	temp = get_path_of_file(dirs, command);
	if (!temp)
		return (free_array(dirs), NULL);
	result = ft_strjoin(temp, "/");
	if (!result)
		return (free_array(dirs), NULL);
	temp = result;
	result = ft_strjoin(temp, command);
	free(temp);
	free_array(dirs);
	return (result);
}

size_t	count_env_vars(t_var *var)
{
	size_t	count;
	t_var	*tmp;

	count = 0;
	tmp = var;
	while (tmp)
	{
		count++;
		tmp = tmp->next;
	}
	return (count);
}

char	**get_env_loop(char **env, t_var *tmp)
{
	int		i;
	char	*temp;

	i = 0;
	while (tmp)
	{
		env[i] = ft_strjoin(tmp->name, "=");
		if (!env[i])
			return (NULL);
		if (tmp->value && tmp->value[0] != '\0')
		{
			temp = env[i];
			env[i] = ft_strjoin(temp, tmp->value);
			free(temp);
			if (!env[i])
				return (NULL);
		}
		tmp = tmp->next;
		i++;
	}
	return (env);
}

char	**get_env(t_shell *shell)
{
	char	**env;
	t_var	*tmp;

	tmp = shell->var;
	env = (char **)ft_calloc(count_env_vars(shell->var) + 1, sizeof(char *));
	if (!env)
		return (NULL);
	env = get_env_loop(env, tmp);
	return (env);
}

void	print_error(t_shell *shell, char *command, char *message, int status)
{
	ft_putstr_fd(command, STDERR_FILENO);
	ft_putstr_fd(message, STDERR_FILENO);
	safely_exit(status, shell, NULL, NULL);
}

int	execute_external(t_cmd *cmd, t_shell *shell)
{
	char	*path;
	int		status;
	char	**env;

	if (!cmd->argv || !cmd->argv[0])
		safely_exit(EXIT_SUCCESS, shell, NULL, NULL);
	if (cmd->argv[0][0] == '.' || cmd->argv[0][0] == '/')
	{
		status = check_file(cmd->argv[0]);
		if (status & 128)
			print_error(shell, cmd->argv[0], ": No such file or directory\n",
				127);
		if (status & 1)
			print_error(shell, cmd->argv[0], ": Is a directory\n", 126);
		else if (status & 2)
			print_error(shell, cmd->argv[0], ": Permission denied\n", 126);
		path = ft_strdup(cmd->argv[0]);
	}
	else
		path = locate_file(cmd->argv[0], get_var_value(shell->var, "PATH"));
	if (!path)
		print_error(shell, cmd->argv[0], ": command not found\n", 127);
	env = get_env(shell);
	execve(path, cmd->argv, env);
	return (safely_exit(EXIT_FAILURE, shell, env, path), EXIT_FAILURE);
}

int	open_temp_file(t_cmd *cmd)
{
	free(cmd->read_path);
	cmd->read_path = get_absolute_path("./.minishell_empty_file");
	cmd->read_fd = open(cmd->read_path, O_RDONLY | O_CREAT | O_TRUNC, 0644);
	if (cmd->read_fd == -1)
		return (0);
	return (1);
}

int	open_normal_file(t_cmd *cmd)
{
	cmd->read_fd = open(cmd->read_path, O_RDONLY);
	if (cmd->read_fd == -1)
	{
		ft_putstr_fd(cmd->read_path, STDERR_FILENO);
		ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
		return (1);
	}
	return (1);
}

int	open_redirs(t_cmd *cmd)
{
	if (cmd->read_path && !ft_strcmp(cmd->read_path, "./.minishell_empty_file")
		&& !open_temp_file(cmd))
		return (1);
	else if (cmd->read_path && !open_normal_file(cmd))
		return (0);
	if (cmd->write_path)
	{
		if (cmd->write_mode == MODE_APPEND)
			cmd->write_fd = open(cmd->write_path, O_WRONLY | O_CREAT | O_APPEND,
					0644);
		else if (cmd->write_mode == MODE_WRITE)
			cmd->write_fd = open(cmd->write_path, O_WRONLY | O_CREAT | O_TRUNC,
					0644);
		if (cmd->write_mode != NOT_SET && cmd->write_fd == -1)
			return (0);
	}
	return (1);
}

size_t	count_cmds(t_cmd *cmd)
{
	size_t	count;

	count = 0;
	while (cmd)
	{
		count++;
		cmd = cmd->next;
	}
	return (count);
}

void	close_files(t_cmd *cmd)
{
	t_cmd	*tmp;

	while (cmd)
	{
		tmp = cmd->next;
		if (cmd->read_fd != NOT_SET && !isatty(cmd->read_fd))
			close(cmd->read_fd);
		if (cmd->write_fd != NOT_SET && !isatty(cmd->write_fd))
			close(cmd->write_fd);
		cmd = tmp;
	}
	unlink("./.minishell_empty_file");
}

int	get_prev_write_fd(t_cmd *cmd, t_cmd *curr)
{
	while (cmd->next != curr)
		cmd = cmd->next;
	if (!cmd->next)
		return (STDIN_FILENO);
	return (cmd->write_fd);
}

int	get_next_read_fd(t_cmd *curr)
{
	if (curr->next)
		return (curr->next->read_fd);
	return (STDOUT_FILENO);
}

void	duplicate_fd(int fd, int std_fd)
{
	if (fd != NOT_SET && !isatty(fd))
	{
		dup2(fd, std_fd);
		close(fd);
	}
}

void	duplicate_read_fd(t_cmd *cmd, int prev_read_fd)
{
	if (cmd->read_fd > 0 && !isatty(cmd->read_fd))
	{
		dup2(cmd->read_fd, STDIN_FILENO);
		close(cmd->read_fd);
	}
	else if (prev_read_fd != NOT_SET)
	{
		dup2(prev_read_fd, STDIN_FILENO);
		close(prev_read_fd);
	}
}

void	duplicate_write_fd(t_cmd *cmd, int pipe_fd[2])
{
	if (cmd->write_fd != NOT_SET && !isatty(cmd->write_fd))
	{
		dup2(cmd->write_fd, STDOUT_FILENO);
		close(cmd->write_fd);
	}
	else if (cmd->next && pipe_fd[1] != NOT_SET)
	{
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[1]);
		if (pipe_fd[0] != NOT_SET && !isatty(pipe_fd[0]))
			close(pipe_fd[0]);
	}
}

void	child_routine(t_shell *shell, t_cmd *cmd, int pipe_fd[2],
		int prev_read_fd)
{
	if (!open_redirs(cmd))
		safely_exit(0, shell, NULL, NULL);
	duplicate_read_fd(cmd, prev_read_fd);
	duplicate_write_fd(cmd, pipe_fd);
	if (is_builtin(cmd->argv[0]))
		exit(execute_builtin(cmd, shell, true));
	else
		exit(execute_external(cmd, shell));
}

void	parent_routine(t_cmd *cmd, int pipe_fd[2], int *prev_read_fd)
{
	if (*prev_read_fd != NOT_SET)
		close(*prev_read_fd);
	if (cmd->next)
	{
		if (pipe_fd[1] != NOT_SET)
			close(pipe_fd[1]);
		*prev_read_fd = pipe_fd[0];
	}
	else
		*prev_read_fd = NOT_SET;
}

pid_t	create_child(t_shell *shell, t_cmd *cmd)
{
	int			pipe_fd[2];
	static int	prev_read_fd = NOT_SET;
	pid_t		pid;

	pipe_fd[0] = NOT_SET;
	pipe_fd[1] = NOT_SET;
	if (cmd->next)
		if (pipe(pipe_fd) == -1)
			return (-1);
	pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0)
		child_routine(shell, cmd, pipe_fd, prev_read_fd);
	else
		parent_routine(cmd, pipe_fd, &prev_read_fd);
	return (pid);
}

int	run_one_builtin(t_cmd *cmd, t_shell *shell)
{
	int	status;

	if (!open_redirs(cmd))
		return (0);
	duplicate_fd(cmd->read_fd, STDIN_FILENO);
	duplicate_fd(cmd->write_fd, STDOUT_FILENO);
	status = execute_builtin(cmd, shell, false);
	close_files(cmd);
	if (shell->var->value)
		free(shell->var->value);
	shell->var->value = ft_itoa(status);
	if (!shell->var->value)
		return (0);
	return (1);
}

void	wait_for_processes(t_shell *shell, pid_t last_pid)
{
	int	status;
	int	i;

	status = 0;
	i = count_cmds(shell->cmd);
	while (i > 0)
	{
		if (waitpid(0, &status, 0) == last_pid)
			set_last_exit_code(shell->var, WEXITSTATUS(status));
		i--;
	}
}

int	execute(t_shell *shell)
{
	t_cmd	*curr;
	pid_t	last_pid;

	curr = shell->cmd;
	if (count_cmds(curr) == 1 && curr->argv && (!ft_strcmp(curr->argv[0], "exit")
			|| !ft_strcmp(curr->argv[0], "export") || !ft_strcmp(curr->argv[0],
				"unset") || !ft_strcmp(curr->argv[0], "cd")))
		return (run_one_builtin(curr, shell));
	while (curr)
	{
		last_pid = create_child(shell, curr);
		if (last_pid == -1)
			return (0);
		curr = curr->next;
	}
	wait_for_processes(shell, last_pid);
	close_files(shell->cmd);
	return (1);
}
