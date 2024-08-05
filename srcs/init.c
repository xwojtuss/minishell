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

t_cmd	*alloc_cmd(int *i, int *command)
{
	t_cmd	*new;

	new = (t_cmd *)malloc(sizeof(t_cmd));
	if (!new)
		return (NULL);
	new->argv = NULL;
	new->read_path = NULL;
	new->write_path = NULL;
	new->next = NULL;
	*command = *i;
	new->argc = 0;
	return (new);
}

int	assign_argv(char **array, t_cmd *new, int command)
{
	int	i;
	int	j;

	i = command;
	j = 0;
	while (array[i] && ft_strcmp(array[i], "|"))
	{
		if (is_redirect(array[i]))
		{
			set_redirect(array[i], new, array[i + 1]);
			i++;
		}
		else
			new->argc++;
		i++;
	}
	new->argv = (char **)ft_calloc(new->argc + 1, sizeof(char *));
	if (!new->argv)
		return (0);
	new->argv[new->argc] = NULL;
	i = command;
	while (array[i] && ft_strcmp(array[i], "|"))
	{
		if (is_redirect(array[i]))
			i++;
		else
		{
			new->argv[j] = ft_strdup(array[i]);
			if (!new->argv[j])
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
	int		i;
	int		command;

	i = 0;
	curr = NULL;
	while (array[i])
	{
		new = alloc_cmd(&i, &command);
		if (!new)
			return (0);
		if (!assign_argv(array, new, command))
			return (0);
		if (!curr)
			shell->cmd = new;
		else
			curr->next = new;
		curr = new;
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