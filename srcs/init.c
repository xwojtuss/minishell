#include "minishell.h"

int	redirect(char *str, char *file, t_cmd *cmd, t_shell *shell)
{
	int		fd;
	char	*abs_path;
	int		status;

	if (file == NULL)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token 'newline'\n",
				STDERR_FILENO); //$? = 2
		return (0);
	}
	abs_path = get_absolute_path(file);
	if (!abs_path)
		return (0);
	fd = get_fd(abs_path, shell->files);
	if (fd == NOT_SET)
	{
		fd = open(abs_path, O_RDWR | O_CREAT, 0644);
		if (fd == -1)
			return (0);
		shell->files = add_file(abs_path, fd, shell->files);
	}
	if (!shell->files)
		return (0);
	if (ft_strcmp(str, ">"))
		status = redir_input(fd, cmd, shell);
	else if (ft_strcmp(str, ">>"))
		status = redir_append(fd, cmd, shell);
	else if (ft_strcmp(str, "<"))
		status = redir_output(fd, cmd, shell);
	else
		status = redir_delimiter(fd, cmd, shell);
	if (!status)
		return (0);
	return (1);
}

t_cmd	*alloc_cmd(int *i, int *argc, int *command)
{
	t_cmd	*new;

	new = (t_cmd *)malloc(sizeof(t_cmd));
	if (!new)
		return (NULL);
	new->args = NULL;
	new->read_fd = STDIN_FILENO;
	new->write_fd = STDOUT_FILENO;
	new->next = NULL;
	*command = *i;
	*argc = 1;
	return (new);
}

int	init_cmd(char **array, t_shell *shell)
{
	t_cmd	*new;
	t_cmd	*curr;
	int		i;
	int		argc;
	int		command;

	/* the first is always the command (exept for a case where the < appears)
	then we take the arguments
	when we reach | or redirects we stop
	if we've reached a redirect we change the read/write fd and open the file if needed
	we alloc another t_cmd and repeat
	if its the first command then the read fd is stdin
	if its the last command then we write to stdout
	the args will always go to the command at the start or before the pipe */
	i = 0;
	curr = NULL;
	while (array[i])
	{
		new = alloc_cmd(&i, &argc, &command);
		if (!new)
			return (0);
		curr->next = new;
		if (i == 0)
			shell->cmd = new;
		curr = new;
		i++;
		while (array[i] && !ft_strcmp(array[i], "|"))
		{
			if (is_redirect(array[i]))
			{
				if (!redirect(array[i], array[i + 1], new, shell))
					return (0);
				i += 2;
			}
			else
				argc++;
			i++;
		}
		new->args = (char **)malloc(sizeof(char *) * (argc + 1));
		if (!new->args)
			return (0);
		new->args[argc] = NULL;
		while (argc >= 0)
		{
			if (!is_redirect(array[i]))
			{
				new->args[argc] = ft_strdup(array[command]);
				if (!new->args[argc])
					return (0);
				argc--;
				command++;
				continue ;
			}
			argc -= 2;
			command += 2;
		}
	}
	return (1);
}

int	init_env(char **envp, t_shell *shell)
{
	t_var *new;
	t_var *curr;
	int i;

	i = 0;
	curr = (t_var *)malloc(sizeof(t_var));
	if (!curr)
		return (0);
	curr->name = ft_strdup("?");
	curr->value = ft_strdup("0");
	curr->next = NULL;
	shell->var = curr;
	while (envp[i])
	{
		new = (t_var *)malloc(sizeof(t_var));
		if (!new)
			return (0);
		new->name = ft_strndup(envp[i], ft_strchr(envp[i], '=') - envp[i]);
		new->value = ft_strdup(ft_strchr(envp[i], '=') + 1); // to change
		new->next = NULL;
		curr->next = new;
		curr = new;
		i++;
	}
	return (1);
}