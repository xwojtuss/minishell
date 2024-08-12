#include "minishell.h"

bool	is_builtin(char *command)
{
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
		ret = ft_exit(cmd->argc, cmd->argv);
	else
		ret = EXIT_FAILURE;
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

char	*locate_file(char *command, char *path)
{
	char	**dirs;
	char	*result;
	char	*temp;
	int		i;

	dirs = ft_split(path, ':');
	if (!dirs)
		return (NULL);
	i = 0;
	while (dirs[i])
	{
		if (check_path_for_file(dirs[i], command))
			break ;
		i++;
	}
	if (!dirs[i])
		return (free_array(dirs), NULL);
	result = ft_strjoin(dirs[i], "/");
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

char	**get_env(t_shell *shell)
{
	char	**env;
	char	*temp;
	t_var	*tmp;
	int		i;

	tmp = shell->var;
	i = 0;
	env = (char **)ft_calloc(count_env_vars(shell->var) + 1, sizeof(char *));
	if (!env)
		return (NULL);
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

int	execute_external(t_cmd *cmd, t_shell *shell)
{
	char	*path;
	char	**env;

	if (!cmd->argv || !cmd->argv[0])
		return (EXIT_FAILURE);
	if (cmd->argv[0][0] == '.' || cmd->argv[0][0] == '/')
		path = ft_strdup(cmd->argv[0]);
	else
		path = locate_file(cmd->argv[0], get_var_value(shell->var, "PATH"));
	if (!path)
	{
		ft_putstr_fd(cmd->argv[0], STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
		safely_exit(127, shell, NULL, NULL);
	}
	env = get_env(shell);
	if (execve(path, cmd->argv, env) == -1)
	{
		safely_exit(NOT_SET, shell, env, path);
		return (EXIT_FAILURE);
	}
	safely_exit(NOT_SET, shell, env, path);
	return (EXIT_FAILURE);
}

int	open_redirs(t_cmd *cmd)
{
	if (cmd->read_path)
	{
		cmd->read_fd = open(cmd->read_path, O_RDONLY);
		if (cmd->read_fd == -1)
			return (0);
	}
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

int	create_pipes(t_shell *shell)
{
	int		pipes[2];
	t_cmd	*temp;

	temp = shell->cmd;
	if (!temp)
		return (0);
	while (temp)
	{
		pipes[0] = NOT_SET;
		pipes[1] = NOT_SET;
		if (pipe(pipes) == -1)
			return (0);
		if (temp == shell->cmd && temp->read_fd == NOT_SET)
			temp->read_fd = STDIN_FILENO;
		else if (temp->read_fd == NOT_SET)
			temp->read_fd = pipes[0];
		if (temp->next == NULL && temp->write_fd == NOT_SET)
			temp->write_fd = STDOUT_FILENO;
		else if (temp->write_fd == NOT_SET)
			temp->write_fd = pipes[1];
		temp = temp->next;
	}
	return (1);
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
}

pid_t	create_child(t_shell *shell, t_cmd *cmd)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (-1);
	}
	if (pid == 0)
	{
		if (cmd->read_fd != STDIN_FILENO)
		{
			dup2(cmd->read_fd, STDIN_FILENO);
			close(cmd->read_fd);
		}
		if (cmd->write_fd != STDOUT_FILENO)
		{
			dup2(cmd->write_fd, STDOUT_FILENO);
			close(cmd->write_fd);
		}
		if (is_builtin(cmd->argv[0]))
			exit(execute_builtin(cmd, shell, true));
		else
			exit(execute_external(cmd, shell));
	}
	return (pid);
}

int	run_one_builtin(t_cmd *cmd, t_shell *shell)
{
	int	status;

	status = execute_builtin(cmd, shell, false);
	if (shell->var->value)
		free(shell->var->value);
	shell->var->value = ft_itoa(WEXITSTATUS(status));
	if (!shell->var->value)
		return (0);
	return (1);
}

int	execute(t_shell *shell)
{
	t_cmd	*curr;
	pid_t	last_pid;
	int		status;
	int		i;

	curr = shell->cmd;
	if (count_cmds(curr) == 1 && is_builtin(curr->argv[0]))
		return (run_one_builtin(curr, shell));
	if (!create_pipes(shell))
		return (0);
	// print_cmd(shell->cmd);
	while (curr)
	{
		last_pid = create_child(shell, curr);
		if (last_pid == -1)
			return (0);
		curr = curr->next;
	}
	status = 0;
	i = count_cmds(shell->cmd);
	while (i > 0)
	{
		if (waitpid(0, &status, 0) == last_pid)
		{
			if (shell->var->value)
				free(shell->var->value);
			shell->var->value = ft_itoa(WEXITSTATUS(status));
		}
		i--;
	}
	close_files(shell->cmd);
	return (1);
}
