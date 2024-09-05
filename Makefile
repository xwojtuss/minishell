COMPILER = cc
CFLAGS = -Wall -Wextra -Werror
LIBFT_FOLDER = libs/libft
INCLUDES = -Iincludes -I${LIBFT_FOLDER}
LIBS = -L${LIBFT_FOLDER} -lft -lreadline -ltinfo

NAME = minishell

SRCS = main.c signals.c pipe.c debug.c alloc.c free.c errors.c init.c check_args.c input.c split.c \
	redir.c files.c execute.c ft_unset.c ft_export.c ft_exit.c ft_echo.c ft_env.c ft_pwd.c ft_cd.c \
	commands.c misc.c env_vars.c fd_management.c open_files.c path.c run_processes.c prompt.c split2.c \
	history.c get_relative_path.c set_redirect.c stdin.c get_env.c create_array.c create_array_2.c \
	assign_argv.c \

SRCS_FOLDER = ${addprefix srcs/,${SRCS}}

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
