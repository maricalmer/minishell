/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_pointer.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 19:30:59 by tmurua            #+#    #+#             */
/*   Updated: 2024/08/06 13:38:53 by tmurua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_print_pointer(void *ptr, const char convert)
{
	int	count;

	count = 0;
	if (ptr == NULL)
		count = count + write(1, "(nil)", 5);
	else
	{
		count = count + write(1, "0x", 2);
		count = count + ft_print_hex((unsigned long)ptr, convert);
	}
	return (count);
}
