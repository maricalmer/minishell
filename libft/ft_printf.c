/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 16:38:53 by tmurua            #+#    #+#             */
/*   Updated: 2024/08/06 13:40:47 by tmurua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_convert(va_list args, const char convert);

int	ft_printf(const char *str, ...)
{
	int		count;
	int		i;
	va_list	args;

	count = 0;
	i = 0;
	va_start(args, str);
	while (str[i])
	{
		if (str[i] == '%')
		{
			count = count + ft_convert(args, str[i + 1]);
			i++;
		}
		else
			count = count + ft_print_char(str[i]);
		i++;
	}
	va_end(args);
	return (count);
}

int	ft_convert(va_list args, const char convert)
{
	int	count;

	count = 0;
	if (convert == 's')
		count = count + ft_print_str(va_arg(args, char *));
	else if (convert == 'c')
		count = count + ft_print_char(va_arg(args, int));
	else if (convert == 'd' || convert == 'i')
		count = count + ft_print_int(va_arg(args, int));
	else if (convert == '%')
		count = count + ft_print_char('%');
	else if (convert == 'p')
		count = count + ft_print_pointer(va_arg(args, void *), convert);
	else if (convert == 'x' || convert == 'X')
		count = count + ft_print_hex(va_arg(args, unsigned int), convert);
	else if (convert == 'u')
		count = count + ft_print_unsigned_int(va_arg(args, unsigned int));
	else
		count = count + ft_print_char(' ');
	return (count);
}
/*
#include <stdio.h>

int	main(void)
{
	int		x;
	void	*ptr_null = NULL;

	x = 42;
	int		*ptr_x = &x;
	printf("%d\n%p\n%p\n", *ptr_x, (void *)ptr_x, (void *)ptr_null);
	ft_printf("%d\n%p\n%p\n", *ptr_x, ptr_x, ptr_null);
}
*/
