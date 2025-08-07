COMPILER = cc
CFLAGS = -Wall -Wextra -Werror
LIBFT_FOLDER = libs/libft
INCLUDES = -Iincludes -I${LIBFT_FOLDER}
LIBS = -L${LIBFT_FOLDER} -lft -lreadline -ltinfo

NAME = minishell

SRCS_ROOT = main.c
SRCS_ARGUMENTS = assign_argv.c check_args.c create_array.c create_array_2.c
SRCS_BUILTINS = ft_cd.c ft_echo.c ft_env.c ft_exit.c ft_export.c ft_pwd.c ft_unset.c
SRCS_EXECUTE = execute.c run_processes.c
SRCS_FILES = fd_management.c files.c get_relative_path.c open_files.c path.c
SRCS_INIT = init.c
SRCS_MEMORY = alloc.c free.c
SRCS_MISC = commands.c debug.c errors.c misc.c signals.c split.c split2.c
SRCS_PARSING = env_vars.c get_env.c history.c input.c pipe.c prompt.c redir.c set_redirect.c stdin.c

SRCS_FOLDER = ${addprefix srcs/,${SRCS_ROOT}} ${addprefix srcs/arguments/,${SRCS_ARGUMENTS}} \
			 ${addprefix srcs/builtins/,${SRCS_BUILTINS}} ${addprefix srcs/execute/,${SRCS_EXECUTE}} \
			 ${addprefix srcs/files/,${SRCS_FILES}} ${addprefix srcs/init/,${SRCS_INIT}} \
			 ${addprefix srcs/memory/,${SRCS_MEMORY}} ${addprefix srcs/misc/,${SRCS_MISC}} \
			 ${addprefix srcs/parsing/,${SRCS_PARSING}}

LIBFT = ${LIBFT_FOLDER}/libft.a

OBJS = ${SRCS_FOLDER:.c=.o}

all: ${NAME}

${NAME}: ${LIBFT} ${OBJS}
	${COMPILER} ${CFLAGS} ${OBJS} -o ${NAME} ${LIBS} ${INCLUDES}
	
srcs/%.o: srcs/%.c
	${COMPILER} ${CFLAGS} -c $^ -o $@ ${INCLUDES}

${LIBFT}:
	make -C ${LIBFT_FOLDER}

clean:
	rm -f ${OBJS} ${OBJSB} ${LIB_OBJECTS}
	make -C ${LIBFT_FOLDER} clean

fclean: clean
	rm -f ${NAME} ${NAMEB}
	make -C ${LIBFT_FOLDER} fclean

re: fclean all

.PHONY: all clean fclean re
