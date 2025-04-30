NAME = libft.a

CFILES =	ft_strlen.c		ft_strlcpy.c	ft_strlcat.c	\
			ft_isalpha.c	ft_isdigit.c	ft_isalnum.c	\
			ft_isascii.c	ft_isprint.c	ft_strncmp.c	\
			ft_atoi.c		ft_tolower.c	ft_toupper.c	\
			ft_strchr.c		ft_strrchr.c	ft_strnstr.c	\
			ft_memset.c		ft_memcpy.c		ft_memmove.c	\
			ft_bzero.c		ft_memchr.c		ft_memcmp.c		\
			ft_strdup.c		ft_calloc.c		ft_putchar_fd.c	\
			ft_putstr_fd.c	ft_putendl_fd.c	ft_putnbr_fd.c	\
			ft_substr.c		ft_strjoin.c	ft_strtrim.c	\
			ft_strmapi.c	ft_striteri.c	ft_split.c		\
			ft_itoa.c		ft_print_char.c	ft_print_str.c	\
			ft_print_int.c	ft_print_unsigned_int.c	ft_print_hex.c \
			ft_print_pointer.c	ft_printf.c	ft_iswhitespace.c \
			ft_atol.c       ft_strndup.c \
			./garbage_collector/gc_strdup.c \
			./garbage_collector/gc_strjoin.c \
			./garbage_collector/gc_calloc.c \
			./garbage_collector/gc_calloc.c \
			./garbage_collector/gc_add_ptr_to_list.c \
			./garbage_collector/gc_free_all.c \
			./garbage_collector/gc_substr.c \
			./garbage_collector/gc_split.c \
			./garbage_collector/gc_itoa.c \
			./garbage_collector/gc_strtrim.c


OBJECTS = $(CFILES:.c=.o)

BONUS_FILES =	ft_lstnew.c		ft_lstadd_front.c	ft_lstsize.c	\
				ft_lstlast.c	ft_lstadd_back.c	ft_lstdelone.c	\
				ft_lstclear.c	ft_lstiter.c		ft_lstmap.c

BONUS_OBJECTS = $(BONUS_FILES:.c=.o)

CFLAGS = -Wall -Wextra -Werror

CC = cc

$(NAME): $(OBJECTS)
	ar -rcs $(NAME) $(OBJECTS)
all: $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

bonus: $(BONUS_OBJECTS)
		ar -rcs $(NAME) $(BONUS_OBJECTS)

clean:
	rm -rf $(NAME) $(OBJECTS) $(BONUS_OBJECTS)
fclean: clean
	rm -rf $(NAME)

re: clean all

.PHONY: all bonus clean fclean re
.SILENT:
