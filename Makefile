COMPILER = clang # TO CHANGE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
CFLAGS = -Wall -Wextra -Werror
LIBFT_FOLDER = libs/libft
INCLUDES = -Iincludes -I${LIBFT_FOLDER}
LIBS = -L${LIBFT_FOLDER} -lft -lreadline -ltinfo

NAME = minishell

SRCS = main.c signals.c pipe.c debug.c alloc.c free.c errors.c init.c check_args.c input.c env.c split.c

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
