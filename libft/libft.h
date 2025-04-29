/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 17:11:17 by tmurua            #+#    #+#             */
/*   Updated: 2024/12/13 21:01:56 by tmurua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <stddef.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdarg.h>

/* ft_printf */

int		ft_print_char(char c);
int		ft_print_str(char *str);
int		ft_print_int(int n);
int		ft_print_unsigned_int(long nb);
int		ft_print_hex(unsigned long nb, const char convert);
int		ft_print_pointer(void *ptr, const char convert);
int		ft_printf(const char *str, ...);

/* String manipulation */
size_t	ft_strlen(const char *s);
size_t	ft_strlcpy(char *dest, const char *src, size_t size);
size_t	ft_strlcat(char *dest, const char *src, size_t size);
char	*ft_strdup(const char *s);
char	*ft_strndup(const char *s, int n);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
void	ft_putchar_fd(char c, int fd);
void	ft_putstr_fd(char *str, int fd);
void	ft_putendl_fd(char *s, int fd);
void	ft_putnbr_fd(int n, int fd);
char	*ft_substr(const char *s, unsigned int start, size_t len);
char	*ft_strjoin(const char *s1, const char *s2);
char	*ft_strtrim(const char *s1, const char *set);
char	*ft_strmapi(const char *s, char (*f)(unsigned int, char));
void	ft_striteri(char *s, void (*f)(unsigned int, char *));
char	**ft_split(const char *s, char c);

/* Character classification */
int		ft_isalpha(int c);
int		ft_isdigit(int c);
int		ft_isalnum(int c);
int		ft_isascii(int c);
int		ft_isprint(int c);
int		ft_iswhitespace(char c);

/* Conversion */
int		ft_toupper(int c);
int		ft_tolower(int c);
int		ft_atoi(const char *str);
char	*ft_itoa(int n);

/* String search */
char	*ft_strchr(const char *s, int c);
char	*ft_strrchr(const char *s, int c);
char	*ft_strnstr(const char *haystack, const char *needle, size_t len);

/* Memory manipulation */
void	*ft_memset(void *b, int c, size_t len);
void	*ft_memcpy(void *dest, const void *src, size_t n);
void	*ft_memmove(void *dest, const void *src, size_t len);
void	ft_bzero(void *s, size_t n);
void	*ft_memchr(const void *s, int c, size_t n);
int		ft_memcmp(const void *s1, const void *s2, size_t n);
void	*ft_calloc(size_t count, size_t size);

/* Bonus functions (lists) */
/*	with addition of garbage_collector fts, t_list serves as storage list for
	all nodes created for mem alloc & will need to be freed with gc_free_all()*/
typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}	t_list;

t_list	*ft_lstnew(void *content);
void	ft_lstadd_front(t_list **lst, t_list *new);
int		ft_lstsize(t_list *lst);
t_list	*ft_lstlast(t_list *lst);
void	ft_lstadd_back(t_list **lst, t_list *new);
void	ft_lstdelone(t_list *lst, void (*del)(void *));
void	ft_lstclear(t_list **lst, void (*del)(void *));
void	ft_lstiter(t_list *lst, void (*f)(void *));
t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));

/* added for later projects */
/* Conversion */
long	ft_atol(const char *str);

/* garbage_collector */
char	*gc_strjoin(t_list **gc_head, const char *s1, const char *s2);
char	*gc_strdup(t_list **gc_head, const char *s1);
void	*gc_calloc(t_list **gc_head, size_t count, size_t size);
void	gc_add_ptr_to_list(t_list **gc_head, void *new_ptr);
void	gc_free_all(t_list *gc_head);
char	*gc_substr(t_list **gc_head, const char *s, unsigned int start,
			size_t len);
char	**gc_split(t_list **gc_head, char const *s, char c);
char	*gc_itoa(t_list **gc_head, int n);
char	*gc_strtrim(t_list **gc_head, const char *s1, const char *set);

#endif
