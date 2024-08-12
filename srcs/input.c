#include "minishell.h"

bool	is_in_quotes(char *input, int index)
{
	int	quote;
	int	i;

	i = 0;
	quote = NOT_SET;
	while (i < index)
	{
		if (input[i] == '\'' && quote == NOT_SET)
			quote = '\'';
		else if (input[i] == '\"' && quote == NOT_SET)
			quote = '\"';
		else if (input[i] == quote)
			quote = NOT_SET;
		i++;
	}
	return (quote != NOT_SET);
}

int	check_empty_pipes(char *input, t_shell *shell)
{
	int	i;
	int	j;

	i = 0;
	while (input[i])
	{
		if (input[i] == '|')
		{
			j = i - 1;
			while (j >= 0 && input[j] && ft_iswhite(input[j]))
				j--;
			if ((j < 0 || input[j] == '|') && !is_in_quotes(input, j))
			{
				ft_putstr_fd("minishell: syntax error near unexpected token '|'\n", STDERR_FILENO);
				if (shell && shell->var && shell->var->value)
					free(shell->var->value);
				shell->var->value = ft_itoa(EXIT_FAILURE);
				add_history((const char *)input);
				return (0);
			}
		}
		i++;
	}
	return (1);
}

char	*check_pipes(char **input, t_shell *shell)
{
	char 	*temp;
	char	*last;
	char	*stdin_line;

	if (!check_empty_pipes(*input, shell))
		return (NULL);
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
			*input = ft_strjoin_delim(temp, ' ', stdin_line);
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
	if (input == NULL)
	{
		rl_clear_history();
		free_var(shell->var);
		exit(EXIT_SUCCESS);
	}
	if (!*input)
		return (free(input));
	input = check_pipes(&input, shell);
	if (!input)
		throw_error_exit(NULL, NULL, NULL, shell->var);
	add_history((const char *)input);
	array = create_array(input, shell->var);
	if (!array)
		throw_error_exit(NULL, NULL, NULL, shell->var);
	if (!check_redirects(array))
		return ;
	if (!init_cmd(array, shell))
		return (free_cmd(shell->cmd), free_array(array));
	free_array(array);
	if (!execute(shell))
		throw_error_exit(NULL, NULL, shell->cmd, shell->var);
	free_cmd(shell->cmd);
}

char	*get_relative_path(char *cwd, t_var *var)
{
	char	*home;
	char	*temp;

	home = get_var_value(var, "HOME");
	if (!home)
		return (NULL);
	if (!ft_strncmp(cwd, home, ft_strlen(home)))
	{
		temp = ft_strjoin("~", cwd + ft_strlen(home));
		if (!temp)
			return (NULL);
		home = temp;
	}
	return (home);
}

char	*construct_prompt(char *cwd, t_var *var)//only a preview, there is a way more efficient way to do this
{
	char	**prompt_array;
	char	*prompt;

	prompt_array = (char **)ft_calloc(15, sizeof(char *));
	if (!prompt_array)
		return (NULL);
	prompt_array[0] = GREEN_COLOR;
	prompt_array[1] = BOLD_TEXT;
	prompt_array[2] = ft_fallback_string(get_var_value(var, "USER"), "user");
	prompt_array[3] = "@";
	prompt_array[4] = ft_fallback_string(get_var_value(var, "HOSTNAME"), "hostname");
	prompt_array[5] = DEFAULT_COLOR;
	prompt_array[6] = NORMAL_TEXT;
	prompt_array[7] = ":";
	prompt_array[8] = BLUE_COLOR;
	prompt_array[9] = BOLD_TEXT;
	prompt_array[10] = ft_fallback_string(get_relative_path(cwd, var), cwd);
	prompt_array[11] = DEFAULT_COLOR;
	prompt_array[12] = NORMAL_TEXT;
	prompt_array[13] = "$ ";
	prompt = ft_strjoin_array(prompt_array);
	if (ft_strcmp(prompt_array[10], cwd) != 0)
		free(prompt_array[10]);
	free(prompt_array);
	return (prompt);
}

void	wait_for_input(char **envp)
{
	char	*input;
	char	*cwd;
	char	*prompt;
	t_shell	shell;

	if (!init_env(envp, &shell))
		throw_error_exit(NULL, NULL, NULL, shell.var);
	while (1)
	{
		cwd = getcwd(NULL, 0);
		if (!cwd)
			throw_error_exit(NULL, NULL, NULL, shell.var);
		prompt = construct_prompt(cwd, shell.var);
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
