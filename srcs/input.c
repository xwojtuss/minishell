#include "minishell.h"

/*
Prepares the input for execution, prelimenary checks are called from here
*/
void	handle_input(char *input, t_shell *shell)
{
	char	**array;
	t_cmd	*cmd;

	shell->cmd = NULL;
	shell->files = NULL;
	if (input == NULL)
	{
		rl_clear_history();
		free_var(shell->var);
		exit(EXIT_SUCCESS);
	}
	if (!*input)
		return (free(input));
	add_history((const char *)input);
	array = create_array(input, shell->var);//if there are quotes - we need to not split at space!!!
	// print_array(array);
	if (!array)
		throw_error_exit(NULL, NULL, NULL, shell->var);
	if (!check_redirects(array))
		return ;
	/* if (!init_cmd(array, shell))
		throw_error_exit(NULL, array, shell->cmd, shell->var); */
	// Initialize the t_cmd structure Bartek
	cmd = Bartek_init_cmd(array);
	if (!cmd)
		throw_error_exit(NULL, array, shell->cmd, shell->var);
	printf("%s\n", get_absolute_path(array[0]));
	// execute()
	Bartek_execute(cmd);
	free_cmd(shell->cmd);
	free_array(array);
	free(cmd);
}

void	wait_for_input(char **envp)
{
	char	*input;
	char	*cwd;
	char	*prompt;
	t_shell	shell;

	rl_clear_history();
	// signal(SIGINT, sigint_exit);
	if (!init_env(envp, &shell))
		throw_error_exit(NULL, NULL, NULL, shell.var);
	while (1)
	{
		cwd = getcwd(NULL, 0);
		if (!cwd)
			throw_error_exit(NULL, NULL, NULL, shell.var);
		prompt = ft_strjoin(cwd, "$ ");
		free(cwd);
		if (!prompt)
			throw_error_exit(NULL, NULL, NULL, shell.var);
		input = readline(prompt);
		free(prompt);
		handle_input(input, &shell);
	}
	free(prompt);
	free_var((&shell)->var);
}