#include "minishell.h"

/* int					init_cmd(char **array, t_shell *shell)
{
	
	the first is always the command (exept for a case where the < appears)
	then we take the arguments
	when we reach | or redirects we stop
	if we've reached a redirect we change the read/write fd and open the file if needed
	we alloc another t_cmd and repeat
	if its the first command then the read fd is stdin
	if its the last command then we write to stdout
	
	the args will always go to the command at the start or before the pipe
	
	t_cmd	*new;
	t_cmd	*curr;
	int		i;
	int		arg_count;
	int		first_arg;

	while (array[i])
	{
		first_arg = NOT_SET;
		arg_count = 0;
		new = (t_cmd *)malloc(sizeof(t_cmd));
		if (!new)
			return (0);
		new->cmd = ft_strdup(array[i]);
		new->args = NULL;
		new->read_fd = STDIN_FILENO;
		new->write_fd = STDOUT_FILENO;
		new->next = NULL;
		curr->next = new;
		if (i == 0)
			shell->cmd = new;
		curr = new;
		i++;
		//while array and no pipe:
		// if redirect then handle it and skip the arguments
		//if pipe then i++
		//else arg++
		//malloc the args, assign them
		while (array[i] && !ft_strcmp(array[i], "|"))
		{
			if (is_redirect)
			{
				//handle redirect
			}
			else
			{
				if (first_arg == NOT_SET)
					first_arg = i;
				arg_count++;
			}
			i++;
		}
	}
} */

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