#include "minishell.h"

/*
redirect will happen right before we fork, we will open and close the file before and after the fork

if the stdout of a previous command is a file then we execute the next command without piping the output of the previous command
*/
int	redirect(char *str, char *file, t_cmd *cmd, t_shell *shell)
{
	int		status;

	if (file == NULL)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token 'newline'\n",
				STDERR_FILENO); //$? = 2
		return (0);
	}
	if (ft_strcmp(str, ">"))
		status = redir_input(file, cmd, shell);
	else if (ft_strcmp(str, ">>"))
		status = redir_append(file, cmd, shell);
	else if (ft_strcmp(str, "<"))
		status = redir_output(file, cmd, shell);
	else
		status = redir_delimiter(file, cmd, shell);
	if (!status)
		return (0);
	return (1);
}

t_cmd	*alloc_cmd(int *i, int *command, char *read_path)
{
	t_cmd	*new;

	new = (t_cmd *)ft_calloc(1, sizeof(t_cmd));
	if (!new)
		return (NULL);
	new->argv = NULL;
	new->read_path = ft_strdup(read_path);
	new->read_fd = NOT_SET;
	new->write_fd = NOT_SET;
	new->write_mode = NOT_SET;
	new->write_path = NULL;
	new->next = NULL;
	if (i && command)
		*command = *i;
	new->argc = 0;
	return (new);
}

t_cmd	*read_stdin_delim(char *delim)
{
	t_cmd	*new;
	char	*stdin_line;
	char	*temp;

	new = alloc_cmd(NULL, NULL, NULL);
	if (!new)
		return (0);
	new->argv = (char **)ft_calloc(3, sizeof(char *));
	if (!(new->argv))
		return (0);
	new->argv[0] = ft_strdup("echo");
	if (!new->argv[0])
		return (0);
	new->argv[1] = ft_strdup("");
	if (!(new->argv[1]))
		return (0);
	new->argc = 2;
	while (true)
	{
		write(STDOUT_FILENO, "> ", 2);
		stdin_line = get_next_line(0);
		if (!stdin_line)
		{
			ft_putstr_fd("\nminishell: warning here-document at this line delimited by end-of-file (wanted '", STDERR_FILENO);
			ft_putstr_fd(delim, STDERR_FILENO);
			ft_putstr_fd("')\n", STDERR_FILENO);
			free_cmd(new);
			return (0);
		}
		if (ft_strlen(stdin_line) == 0)
		{
			free(stdin_line);
			continue ;
		}
		if (!ft_strncmp(stdin_line, delim, ft_max(ft_strlen(delim), ft_strlen(stdin_line) - 1)))
			break ;
		temp = new->argv[1];
		new->argv[1] = ft_strjoin(temp, stdin_line);
		free(temp);
		free(stdin_line);
		if (!(new->argv[1]))
			return (0);
	}
	if (new && new->argv && new->argv[1] && ft_strlen(new->argv[1]) > 0)
		new->argv[1][ft_strlen(new->argv[1]) - 1] = '\0';
	free(stdin_line);
	new->argv[2] = NULL;
	return (new);
}

int	set_redirect(char *str, t_cmd **cmd, char *file)
{
	t_cmd	*new;

	if (!ft_strcmp(str, ">"))
	{
		if ((*cmd)->write_path)
		{
			close(open((*cmd)->write_path, O_WRONLY | O_CREAT, 0644));
			free((*cmd)->write_path);
		}
		(*cmd)->write_mode = MODE_WRITE;
		(*cmd)->write_path = get_absolute_path(file);
		if ((*cmd)->write_path && does_file_exist((*cmd)->write_path) && check_file((*cmd)->write_path) & 8)
		{
			ft_putstr_fd(file, STDERR_FILENO);
			ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
			return (0);
		}
	}
	else if (!ft_strcmp(str, ">>"))
	{
		if ((*cmd)->write_path)
		{
			close(open((*cmd)->write_path, O_WRONLY | O_CREAT, 0644));
			free((*cmd)->write_path);
		}
		(*cmd)->write_mode = MODE_APPEND;
		(*cmd)->write_path = get_absolute_path(file);
		if ((*cmd)->write_path && does_file_exist((*cmd)->write_path) && check_file((*cmd)->write_path) & 8)
		{
			ft_putstr_fd(file, STDERR_FILENO);
			ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
			return (0);
		}
	}
	else if (!ft_strcmp(str, "<"))
	{
		(*cmd)->read_path = get_absolute_path(file);
		if (!does_file_exist((*cmd)->read_path))
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd(file, STDERR_FILENO);
			ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
			return (0);
		}
	}
	else if (!ft_strcmp(str, "<<"))
	{
		new = read_stdin_delim(file);
		if (!new)
			return (0);
		new->next = *cmd;
		*cmd = new;
	}
	return (1);
}

char	*copy_without_quotes(char *s)
{
	char	*result;
	int		wait_for;
	int		i;

	result = (char *)ft_calloc(ft_strlen(s) + 1, sizeof(char));
	if (!result)
		return (NULL);
	wait_for = NOT_SET;
	i = 0;
	while (*s)
	{
		if (*s == '\'' && wait_for == NOT_SET)
			wait_for = '\'';
		else if (*s == '\"' && wait_for == NOT_SET)
			wait_for = '\"';
		else if (*s == wait_for)
			wait_for = NOT_SET;
		else
		{
			result[i] = *s;
			i++;
		}
		s++;
	}
	return (result);
}

int	assign_argv(char **array, t_cmd **new, int command)
{
	int	i;
	int	j;

	i = command;
	j = 0;
	while (array[i] && ft_strcmp(array[i], "|"))
	{
		if (is_redirect(array[i]))
			i++;
		else
			(*new)->argc++;
		i++;
	}
	(*new)->argv = (char **)ft_calloc((*new)->argc + 1, sizeof(char *));
	if (!((*new)->argv))
		return (0);
	(*new)->argv[(*new)->argc] = NULL;
	i = command;
	while (array[i] && ft_strcmp(array[i], "|"))
	{
		if (is_redirect(array[i]))
		{
			if (!set_redirect(array[i], new, array[i + 1]))
				return (0);
			i++;
		}
		else
		{
			(*new)->argv[j] = copy_without_quotes(array[i]);
			if (!((*new)->argv[j]))
				return (0);
			j++;
		}
		i++;
	}
	return (1);
}

int	init_cmd(char **array, t_shell *shell)
{
	t_cmd	*new;
	t_cmd	*curr;
	char	*next_read_path;
	int		i;
	int		command;

	i = 0;
	curr = NULL;
	next_read_path = NULL;
	while (array[i])
	{
		new = alloc_cmd(&i, &command, next_read_path);
		if (!new)
			return (0);
		if (assign_argv(array, &new, command))
		{
			if (!curr)
				shell->cmd = new;
			else
				curr->next = new;
			if (new->next)
				curr = new->next;
			else
				curr = new;
		}
		else
		{
			set_last_exit_code(shell->var, EXIT_FAILURE);
			if (!shell->cmd)
				free_cmd(new);
			next_read_path = "./.minishell_empty_file";
		}		
		while (array[i] && ft_strcmp(array[i], "|"))
			i++;
		if (array[i])
			i++;
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
	if (!curr->name)
		return (0);
	curr->value = ft_strdup("0");
	if (!curr->value)
		return (0);
	curr->next = NULL;
	shell->var = curr;
	while (envp[i])
	{
		new = (t_var *)malloc(sizeof(t_var));
		if (!new)
			return (0);
		new->name = ft_strndup(envp[i], ft_strchr(envp[i], '=') - envp[i]);
		if (!new->name)
			return (0);
		new->value = ft_strdup(ft_strchr(envp[i], '=') + 1); // to change
		if (!new->value)
			return (0);
		new->next = NULL;
		curr->next = new;
		curr = new;
		i++;
	}
	return (1);
}