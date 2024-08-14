#include "minishell.h"

static void	delete_var(t_var **var_list, const char *name)
{
	t_var	*current;
	t_var	*prev;

	current = *var_list;
	prev = NULL;
	while (current != NULL)
	{
		if (ft_strcmp(current->name, (char *)name) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				*var_list = current->next;
			free(current->name);
			free(current->value);
			free(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}

void	Bartek_unset(t_cmd *cmd, t_shell *shell)
{
	int i;
	t_var *var_list = shell->var;

	i = 1;
	if (cmd->args[1] == NULL)
		return ;

	while (cmd->args[i] != NULL)
	{
		delete_var(&var_list, cmd->args[i]);
		i++;
	}
	shell->var = var_list;
}
