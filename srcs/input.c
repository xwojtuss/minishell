#include "minishell.h"

char	*check_pipes(char **input)
{
	char 	*temp;
	char	*last;
	char	*stdin_line;

	while (true)
	{
		temp = *input;
		last = ft_strrchr(*input, '|');
		if (!last)
			break ;
		last++;
		while (*last && ft_iswhite(*last))
			last++;
		if (*last)
			break ;
		else
		{
			write(STDOUT_FILENO, "> ", 2);
			stdin_line = get_next_line(0);
			if (!stdin_line)
				return (NULL);
			if (ft_strlen(stdin_line) == 0)
			{
				free(stdin_line);
				continue ;
			}
			if (stdin_line[ft_strlen(stdin_line) - 1] == '\n')
				stdin_line[ft_strlen(stdin_line) - 1] = '\0';
			*input = ft_strjoin(temp, stdin_line);
			free(temp);
			free(stdin_line);
			if (!*input)
				return (NULL);
		}
	}
	return (*input);
}

/*
Prepares the input for execution, prelimenary checks are called from here
*/
void	handle_input(char *input, t_shell *shell)
{
	char	**array;

	shell->cmd = NULL;
	shell->files = NULL;
	if (input == NULL)
	{
		rl_clear_history();
		free_var(shell->var);
		free_files(shell->files);
		exit(EXIT_SUCCESS);
	}
	if (!*input)
		return (free(input));
	if (!check_pipes(&input))
		return (free(input));
	add_history((const char *)input);
	array = create_array(input, shell->var);
	if (!array)
		throw_error_exit(NULL, NULL, NULL, shell->var);
	// print_array(array);
	if (!check_redirects(array))
		return ;
	/* if (!init_cmd(array, shell))
		throw_error_exit(NULL, array, shell->cmd, shell->var); */
	// Initialize the t_cmd structure Bartek
	/* cmd = Bartek_init_cmd(array);
	if (!cmd)
		throw_error_exit(NULL, array, shell->cmd, shell->var);
	printf("%s\n", get_absolute_path(array[0])); */
	// execute()
	/* Bartek_execute(cmd, shell); */
	if (!init_cmd(array, shell))
		throw_error_exit(NULL, array, shell->cmd, shell->var);
	free_array(array);
	int i = 0;
	t_cmd *tmp = shell->cmd;
	while (tmp)
	{
		i = 0;
		while (i < tmp->argc)
		{
			printf("cmd->argv[%d]: %s\n", i, tmp->argv[i]);
			i++;
		}
		tmp = tmp->next;
	}
	/* if (!execute())
		throw_error_exit(NULL, NULL, shell->cmd, shell->var); */
	free_cmd(shell->cmd);
	free_array(array);
}

void	wait_for_input(char **envp)
{
	char	*input;
	char	*cwd;
	char	*prompt;
	t_shell	shell;

	rl_clear_history();
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
	free_files((&shell)->files);
}