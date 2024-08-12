COMPILER = cc
CFLAGS = -Wall -Wextra -Werror -fPIC

LIB_NAME = libft.a

LIB_SRCS = ft_isalpha.c ft_toupper.c ft_isdigit.c ft_tolower.c ft_isalnum.c ft_strchr.c ft_strchrs.c ft_isascii.c ft_strncmp.c ft_strrchr.c ft_memchr.c ft_isprint.c \
		ft_memcmp.c ft_strlen.c ft_strnstr.c ft_memset.c ft_bzero.c ft_memcpy.c ft_atoi.c ft_atoi_long.c ft_memmove.c ft_strlcpy.c ft_strlcat.c ft_calloc.c \
		ft_strdup.c ft_strndup.c ft_substr.c ft_strjoin.c ft_strtrim.c ft_split.c ft_itoa.c ft_strmapi.c ft_striteri.c ft_putstr_fd.c ft_putchar_fd.c ft_putendl_fd.c ft_putnbr_fd.c \
		get_next_line.c ft_put.c ft_putptr.c ft_printf.c ft_strcmp.c ft_strcpy.c ft_strtok_r.c ft_strcspn.c ft_strspn.c \
		ft_strcat.c ft_iswhite.c ft_max.c ft_min.c ft_abs.c ft_strjoin_array_delim.c ft_strjoin_array.c ft_fallback_string.c \
		ft_strjoin_delim.c

BONUS_SRCS = ft_lstnew.c ft_lstadd_front.c ft_lstsize.c ft_lstlast.c ft_lstadd_back.c ft_lstdelone.c ft_lstclear.c ft_lstmap.c ft_lstiter.c

LIB_OBJECTS = ${LIB_SRCS:.c=.o}
BONUS_OBJECTS = ${BONUS_SRCS:.c=.o}

all: ${LIB_NAME}

${LIB_NAME}: ${LIB_OBJECTS}
	ar rc ${LIB_NAME} ${LIB_OBJECTS}

bonus: ${BONUS_OBJECTS}
	ar rc ${LIB_NAME} ${BONUS_OBJECTS}

%.o: %.c
	${COMPILER} ${CFLAGS} -c $< -o $@ -I.

clean:
	rm -f ${LIB_OBJECTS} ${BONUS_OBJECTS}

fclean: clean
	rm -f ${LIB_NAME}

re: fclean all
