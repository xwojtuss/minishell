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

t_cmd	*alloc_cmd(int *i, int *argc, int *command)
{
	t_cmd	*new;

	new = (t_cmd *)malloc(sizeof(t_cmd));
	if (!new)
		return (NULL);
	new->args = NULL;
	new->read_path = NULL;
	new->write_path = NULL;
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
	the args will always go to the command at the start or before the pipe
	
	<< w | cat !!!!!!!!!!!!!!!!!!!!!!!!!!!!
	*/
	i = 0;
	curr = NULL;
	while (array[i])
	{
		new = alloc_cmd(&i, &argc, &command);
		if (!new)
			return (0);
		if (i == 0)
			shell->cmd = new;
		else
			curr->next = new;
		curr = new;
		i++;
		while (array[i] && !ft_strcmp(array[i], "|"))
		{
			if (is_redirect(array[i]))
			{
				/* if (!redirect(array[i], array[i + 1], new, shell))
					return (0); */
				i += 2;
			}
			else
				argc++;
			i++;
		}
		new->args = (char **)malloc(sizeof(char *) * (argc + 2));
		if (!new->args)
			return (0);
		new->args[argc + 1] = NULL;
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

	t_cmd *cmd = shell->cmd;
	while (cmd)
	{
		printf("cmd->args[0]: %s\n", cmd->args[0]);
		cmd = cmd->next;
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