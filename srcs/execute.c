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
	int		i;

	dirs = ft_split(path, ':');
	if (!dirs)
		return (NULL);
	i = 0;
	while (dirs[i])
	{
		if (check_path_for_file(dirs[i++], command))
			break ;
	}
	if (!dirs[i])
		return (free_array(dirs), NULL);
	result = ft_strjoin(dirs[i], command);
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
			env[i] = ft_strjoin(env[i], tmp->value);
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
	execve(path, cmd->argv, env);
	free(path);
	free_array(env);
	return (EXIT_FAILURE);
}  

int	create_child(t_cmd *curr, t_shell *shell, int pipe_fd[2])
{
	pid_t	pid;
	int		pipe_prev[2];
	
	pid = fork();
	if (pid == -1)
		return (0);
	if (pid == 0)
	{
		if (shell->cmd != curr)
		{
			if (curr->read_fd != NOT_SET)
				dup2(curr->read_fd, STDIN_FILENO);
			else
				dup2(pipe_prev[0], STDIN_FILENO);
			close(pipe_prev[0]);
			close(pipe_prev[1]);
		}
		if (curr->next)
		{
			if (curr->write_fd != NOT_SET)
				dup2(curr->write_fd, STDOUT_FILENO);
			else
				dup2(pipe_fd[1], STDOUT_FILENO);
			close(pipe_fd[0]);
			close(pipe_fd[1]);
		}
		if (is_builtin(curr->argv[0]))
			exit(execute_builtin(curr, shell));
		else
			exit(execute_external(curr, shell));
	}
	else
	{
		if (shell->cmd != curr)
		{
			close(pipe_prev[0]);
			close(pipe_prev[1]);
		}
		if (curr->next)
		{
			pipe_prev[0] = pipe_fd[0];
			pipe_prev[1] = pipe_fd[1];
		}
	}
	if (!curr->next && shell->cmd != curr)
	{
		close(pipe_prev[0]);
		close(pipe_prev[1]);
	}
	return (1);
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

int	execute(t_shell *shell)
{
	t_cmd	*curr;
	int		pipes[2];

	curr = shell->cmd;
	if (!create_pipes(shell, pipes))
		return (0);
	while (curr)
	{
		if (!create_child(curr, shell, pipes))
			return (0);
		curr = curr->next;
	}
	while (curr)
	{
		wait(NULL);
		curr = curr->next;
	}
	close_files(shell->cmd);
	return (1);
}
