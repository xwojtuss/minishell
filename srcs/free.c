#include "minishell.h"

/*
A safe way to exit the program, freeing all the allocated memory
If the exit code is NOT_SET, it will not exit
*/
void	safely_exit(int exit_code, t_shell *shell, char **array, char *input)
{
	rl_clear_history();
	if (shell && shell->var)
		free_var(shell->var);
	if (shell && shell->cmd)
		free_cmd(shell->cmd);
	if (array)
		free_array(array);
	if (input)
		free(input);
	if (exit_code != NOT_SET)
		exit(exit_code);
}

/*
Frees the array of strings, the array must be NULL terminated
*/
void	free_array(char **array)
{
	int i;

	i = 0;
	while (array[i])
	{
		if (array[i])
			free(array[i]);
		i++;
	}
	if (array)
		free(array);
}

/*
Free the var linked list
*/
void	free_var(t_var *var)
{
	t_var *tmp;

	while (var)
	{
		tmp = var->next;
		if (var->name)
			free(var->name);
		if (var->value)
			free(var->value);
		free(var);
		var = tmp;
	}
}

/*
Free the cmd linked list
*/
void	free_cmd(t_cmd *cmd)
{
	t_cmd *tmp;

	while (cmd)
	{
		tmp = cmd->next;
		if (cmd->argv)
			free_array(cmd->argv);
		if (cmd->read_path)
			free(cmd->read_path);
		if (cmd->write_path)
			free(cmd->write_path);
		free(cmd);
		cmd = tmp;
	}
}