#include "minishell.h"

int					init_cmd(char **array, t_shell *shell)
{
	t_cmd			*new;
	t_cmd			*curr;
	int				i;

	i = 0;
	curr = NULL;
	shell->cmd = NULL;
	while (array[i])
	{
		new = (t_cmd *)malloc(sizeof(t_cmd));
		if (!new)
			return (0);
		new->cmd = NULL;
		new->args = NULL;
		new->next = NULL;
		if (i != 0)
			curr->next = new;
		else
			shell->cmd = new;
		curr = new;
		i++;
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