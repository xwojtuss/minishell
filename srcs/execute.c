#include "minishell.h"

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
