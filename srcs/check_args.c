#include "minishell.h"

bool	is_empty_or_pipe(char *str)
{
	if (!str)
		return (true);
	if (!*str || !ft_strcmp(str, "|"))
		return (true);
	return (false);
}

/*
A function to deal with the redirections
Things to look out for:
- if there is a pipe but there is no command after it - read stdin to get the command until there is something other than a whitespace, append it as the last argument, start over
- if there is a redirect << it will do its job before anything else
- the redirect <, >> with nothing before it will do nothing but return as successful (it should check if the file exists though)
- the redirect > with nothing before it will write (and overwrite!!!) to a given file and return as successful

Something to remember: the << only cares about the next argument, the argument after that will be passed to anything that is before the <<
*/

/*
Prelimenary check to see if the redirects are correct
It will check for the following:
- if there is a redirect < or > or >> and after it there is no argument for a path - write "minishell: syntax error near unexpected token 'newline'"

it frees the array and returns false if there is an error
*/
bool	check_redirects(t_shell *shell, char **array)
{
	int	i;

	i = 0;
	while (array[i])
	{
		if ((ft_strcmp(array[i], "<") == 0 || ft_strcmp(array[i], ">") == 0 || ft_strcmp(array[i], ">>") == 0 || ft_strcmp(array[i], "<<") == 0) && !array[i + 1])
		{
			ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n", STDERR_FILENO);
			set_last_exit_code(shell->var, 2);
			free_array(array);
			return (false);
		}
		if (!ft_strcmp(array[i], ">>>"))
		{
			ft_putstr_fd("minishell: syntax error near unexpected token `>'\n", STDERR_FILENO);
			set_last_exit_code(shell->var, 2);
			free_array(array);
			return (false);
		}
		i++;
	}
	return (true);
}