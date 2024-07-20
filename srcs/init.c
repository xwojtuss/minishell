#include "minishell.h"

bool	is_redirect(char *str)
{
	if (!ft_strcmp(str, ">") || !ft_strcmp(str, ">>") || !ft_strcmp(str, "<<") || !ft_strcmp(str, "<"))
		return (true);
	return (false);
}

/*
Calculates the length of the number of characters before the 1st occurrence of character present in both the string.
*/
size_t	ft_strcspn(const char *str1, const char *str2)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (str1[i])
	{
		j = 0;
		while (str2[j])
		{
			if (str1[i] == str2[j])
				break ;
			j++;
		}
		i++;
	}
	return (i);
}

size_t ft_strspn(const char *str1, const char *str2)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (str1[i])
	{
		j = 0;
		while (str2[j])
		{
			if (str1[i] == str2[j])
				break ;
			j++;
		}
		if (!str2[j])
			return (i);
		i++;
	}
	return (i);
}

char	*ft_strtok_r(char *str, const char *delim, char **saveptr)
{
	char	*end;

	if (!str)
		str = *saveptr;
	if (!*str)
	{
		*saveptr = str;
		return (NULL);
	}
	str += ft_strspn(str, delim);
	if (!*str)
	{
		*saveptr = str;
		return (NULL);
	}
	end = str + ft_strcspn(str, delim);
	if (!*end)
	{
		*saveptr = end;
		return (str);
	}
	*end = '\0';
	*saveptr = end + 1;
	return (str);
}

char	*remove_dots(char *final, char *path)
{
	char *token;
    char *rest = path;
    char temp[PATH_MAX] = {0};
    char *last_slash = NULL;

    while ((token = ft_strtok_r(rest, "/", &rest))) {
        if (ft_strcmp(token, ".") == 0) {
            continue;
        } else if (ft_strcmp(token, "..") == 0) {
            if (last_slash) {
                *last_slash = '\0';
                last_slash = ft_strrchr(temp, '/');
            }
        } else {
            ft_strlcat(temp, "/", PATH_MAX - strlen(temp));
            ft_strlcat(temp, token, PATH_MAX - strlen(temp));
            last_slash = ft_strrchr(temp, '/');
        }
    }
    ft_strlcpy(final, temp, PATH_MAX);
    return (final);
}

char	*get_absolute_path(char *path)
{
	char	*result;
	char	*final;

	if (path[0] == '/')
		return (ft_strdup(path));
	result = (char *)ft_calloc(PATH_MAX, sizeof(char));
	if (!result)
		return (NULL);
	if (!getcwd(result, PATH_MAX))
	{
		free(result);
		return (NULL);
	}
	ft_strlcat(result, "/", PATH_MAX - ft_strlen(result));
	ft_strlcat(result, path, PATH_MAX - ft_strlen(result));
	final = (char *)ft_calloc(PATH_MAX, sizeof(char));
	if (!final)
		return (NULL);
	remove_dots(final, result);
	free(result);
	return (final);
}

t_files	*add_file(char *file, int fd, t_files *files)
{
	t_files	*new;

	new = (t_files *)malloc(sizeof(t_files));
	if (!new)
		return (NULL);
	new->path = get_absolute_path(file);
	if (!new->path)
		return (NULL);
	new->fd = fd;
	new->next = files;
	return (new);
}

int	get_fd(char *file, t_files *files)
{
	while (files)
	{
		if (!ft_strcmp(files->path, file))
			return (files->fd);
		files = files->next;
	}
	return (NOT_SET);
}

int	redir_input(int fd, t_cmd *cmd, t_shell *shell)
{
	(void)fd;
	(void)cmd;
	(void)shell;
	return (1);
}

int	redir_output(int fd, t_cmd *cmd, t_shell *shell)
{
	(void)fd;
	(void)cmd;
	(void)shell;
	return (1);
}

int	redir_append(int fd, t_cmd *cmd, t_shell *shell)
{
	(void)fd;
	(void)cmd;
	(void)shell;
	return (1);
}

int	redir_delimiter(int fd, t_cmd *cmd, t_shell *shell)
{
	(void)fd;
	(void)cmd;
	(void)shell;
	return (1);
}

int		redirect(char *str, char *file, t_cmd *cmd, t_shell *shell)
{
	int fd;
	char *abs_path;
	int status;

	if (file == NULL)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token 'newline'\n", STDERR_FILENO);//$? = 2
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

int					init_cmd(char **array, t_shell *shell)
{
	
	/* the first is always the command (exept for a case where the < appears)
	then we take the arguments
	when we reach | or redirects we stop
	if we've reached a redirect we change the read/write fd and open the file if needed
	we alloc another t_cmd and repeat
	if its the first command then the read fd is stdin
	if its the last command then we write to stdout
	
	the args will always go to the command at the start or before the pipe */
	
	t_cmd	*new;
	t_cmd	*curr;
	int		i;
	int		argc;
	int		command;

	i = 0;
	while (array[i])
	{
		command = i;
		argc = 1;
		new = (t_cmd *)malloc(sizeof(t_cmd));
		if (!new)
			return (0);
		new->args = NULL;
		new->read_fd = STDIN_FILENO;
		new->write_fd = STDOUT_FILENO;
		new->next = NULL;
		curr->next = new;
		if (i == 0)
			shell->cmd = new;
		curr = new;
		i++;
		while (array[i] && !ft_strcmp(array[i], "|"))
		{
			if (is_redirect(array[i]))
			{
				redirect(array[i], array[i + 1], new, shell);
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
	t_var			*new;
	t_var			*curr;
	int				i;

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
		new->value = ft_strdup(ft_strchr(envp[i], '=') + 1);//to change
		new->next = NULL;
		curr->next = new;
		curr = new;
		i++;
	}
	return (1);
}