#include "minishell.h"

bool	is_builtin(char *command)
{
	if (!ft_strcmp(command, "echo") || !ft_strcmp(command, "cd") || !ft_strcmp(command, "pwd") || !ft_strcmp(command, "export") || !ft_strcmp(command, "unset") || !ft_strcmp(command, "env") || !ft_strcmp(command, "exit"))
		return (true);
	return (false);
}

int	execute_builtin(t_cmd *cmd, t_shell *shell)
{
	if (!cmd->argv || !cmd->argv[0])
		return (EXIT_FAILURE);
	if (!ft_strcmp(cmd->argv[0], "echo"))
		return (ft_echo(cmd->argc, cmd->argv));
	if (!ft_strcmp(cmd->argv[0], "cd"))
		return (ft_cd(cmd->argc, cmd->argv));
	if (!ft_strcmp(cmd->argv[0], "pwd"))
		return (ft_pwd(cmd->argc, cmd->argv));
	if (!ft_strcmp(cmd->argv[0], "export"))
		return (ft_export(cmd->argc, cmd->argv, shell));
	if (!ft_strcmp(cmd->argv[0], "unset"))
		return (ft_unset(cmd->argc, cmd->argv, shell));
	if (!ft_strcmp(cmd->argv[0], "env"))
		return (ft_env(cmd->argc, cmd->argv,  shell));
	if (!ft_strcmp(cmd->argv[0], "exit"))
		return (ft_exit(cmd->argc, cmd->argv));
	return (0);
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
	if(!result)
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
	int 	i;

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
		return (EXIT_FAILURE);
	env = get_env(shell);
	printf("executing %s\n", path);
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
			cmd->write_fd = open(cmd->write_path, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else if (cmd->write_mode == MODE_WRITE)
			cmd->write_fd = open(cmd->write_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (cmd->write_mode != NOT_SET && cmd->write_fd == -1)
			return (0);
	}
	return (1);
}

int create_pipes(t_shell *shell, int pipes[2])
{
	t_cmd	*curr;

	curr = shell->cmd;
	while (curr)
	{
		if (curr->next)
		{
			if (pipe(pipes) == -1)
				return (0);
		}
		if (!open_redirs(curr))
			return (0);
		curr = curr->next;
	}
	return (1);
}

void	close_files(t_cmd *cmd)
{
	t_cmd	*tmp;

	while (cmd)
	{
		tmp = cmd->next;
		if (cmd->read_fd != NOT_SET)
			close(cmd->read_fd);
		if (cmd->write_fd != NOT_SET)
			close(cmd->write_fd);
		cmd = tmp;
	}
}

pid_t	create_child(t_shell *shell, t_cmd *cmd, int pipes[2], int prev_pipes[2])
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		close(pipes[0]);
		close(pipes[1]);
		close(prev_pipes[0]);
		close(prev_pipes[1]);
		return (-1);
	}
	if (pid == 0)
	{
		if (prev_pipes[0] != STDIN_FILENO)
		{
			dup2(prev_pipes[0], STDIN_FILENO);
			close(prev_pipes[0]);
			close(prev_pipes[1]);
		}
		if (pipes[1] != STDOUT_FILENO)
		{
			dup2(pipes[1], STDOUT_FILENO);
			close(pipes[0]);
			close(pipes[1]);
		}
		if (is_builtin(cmd->argv[0]))
			exit(execute_builtin(cmd, shell));
		else
			exit(execute_external(cmd, shell));
	}
	close(prev_pipes[0]);
	close(prev_pipes[1]);
	return (pid);
}

int	execute(t_shell *shell)
{
	t_cmd	*curr;
	pid_t	last_pid;
	int		status;
	int		pipes[2];
	int		prev_pipe[2];

	pipes[0] = STDIN_FILENO;
	pipes[1] = STDOUT_FILENO;
	prev_pipe[0] = STDIN_FILENO;
	prev_pipe[1] = STDOUT_FILENO;
	curr = shell->cmd;
	if (!create_pipes(shell, pipes))
		return (0);
	while (curr)
	{
		last_pid = create_child(shell, curr, pipes, prev_pipe);
		if (last_pid == -1)
			return (0);
		curr = curr->next;
	}
	status = 0;
	waitpid(last_pid, &status, 0);
	printf("exit status: %d\n", WEXITSTATUS(status));
	while (wait(NULL) > 0);
	close_files(shell->cmd);
	return (1);
}
