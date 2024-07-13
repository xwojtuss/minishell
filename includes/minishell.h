#ifndef MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>

extern int	g_signum;

void sig_handler(int num);
void	handle_input(const char *input);

#endif
