/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wkornato <wkornato@student.42warsaw.pl>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 13:52:16 by wkornato          #+#    #+#             */
/*   Updated: 2024/08/12 21:09:02 by wkornato         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <fcntl.h>
# include <limits.h>
# include <stdarg.h>
# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}					t_list;

int					ft_min(int x, int y);
int					ft_max(int x, int y);
int					ft_abs(int n);
int					ft_isalpha(int c);
int					ft_toupper(int ch);
int					ft_isdigit(int c);
int					ft_tolower(int ch);
int					ft_isalnum(int c);
int					ft_iswhite(char c);
char				*ft_strchr(const char *str, int c);
char				*ft_strchrs(const char *str, const char *c);
char				*ft_strchrs_mf(const char *str, const char *c);
int					ft_isascii(int c);
char				*ft_strrchr(const char *str, int ch);
size_t				ft_strspn(const char *str1, const char *str2);
size_t				ft_strcspn(const char *str1, const char *str2);
char				*ft_strtok_r(char *str, const char *delim, char **saveptr);
int					ft_isprint(int c);
int					ft_strncmp(const char *str1, const char *str2, size_t num);
int					ft_strcmp(char *s1, char *s2);
size_t				ft_strlen(const char *s);
void				*ft_memchr(const void *str, int c, size_t n);
void				*ft_memset(void *str, int c, size_t n);
int					ft_memcmp(const void *str1, const void *str2, size_t n);
void				ft_bzero(void *s, size_t n);
char				*ft_strnstr(const char *big, const char *little,
						size_t len);
void				*ft_memcpy(void *dest, const void *src, size_t n);
int					ft_atoi(const char *str);
long int			ft_atoi_long(const char *str);
void				*ft_memmove(void *dest, const void *src, size_t count);
size_t				ft_strlcpy(char *dst, const char *src, size_t size);
size_t				ft_strlcat(char *dest, const char *src, unsigned int size);
void				*ft_calloc(size_t nmemb, size_t size);
char				*ft_strdup(const char *s);
char				*ft_strndup(const char *s, int length);

char				*ft_strcpy(char *dest, char *src);
char				*ft_strcat(char *dest, char *src);

char				*ft_fallback_string(char *str, const char *fallback);
char				*ft_substr(char const *s, unsigned int start, size_t len);
char				*ft_strjoin(char const *s1, char const *s2);
char				*ft_strjoin_delim(char const *s1, char delim,
						char const *s2);
char				*ft_strjoin_array(char **array);
char				*ft_strjoin_array_delim(char **array, char delim);
char				*ft_strtrim(char const *s1, char const *set);
char				**ft_split(char const *s, char c);
char				*ft_itoa(int n);
char				*ft_strmapi(char const *s, char (*f)(unsigned int, char));
void				ft_striteri(char *s, void (*f)(unsigned int, char *));
void				ft_putchar_fd(char c, int fd);
void				ft_putstr_fd(char *s, int fd);
void				ft_putendl_fd(char *s, int fd);
void				ft_putnbr_fd(int n, int fd);

t_list				*ft_lstnew(void *content);
void				ft_lstadd_front(t_list **lst, t_list *new);
int					ft_lstsize(t_list *lst);
t_list				*ft_lstlast(t_list *lst);
void				ft_lstadd_back(t_list **lst, t_list *new);
void				ft_lstdelone(t_list *lst, void (*del)(void *));
void				ft_lstclear(t_list **lst, void (*del)(void *));
void				ft_lstiter(t_list *lst, void (*f)(void *));
t_list				*ft_lstmap(t_list *lst, void *(*f)(void *),
						void (*del)(void *));

char				*get_next_line(int fd);

int					ft_printf(const char *str, ...);
int					ft_putchar(int to_print);
int					ft_putstr(char *string);
int					ft_putnbr(long int n, int *count, int is_signed);
int					ft_puthex(long int n, int *count, char offset);
int					ft_putptr(void *ptr);

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 2024
# endif
#endif
