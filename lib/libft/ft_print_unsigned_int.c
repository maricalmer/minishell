/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_unsigned_int.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 20:55:32 by tmurua            #+#    #+#             */
/*   Updated: 2024/08/06 13:40:29 by tmurua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_print_unsigned_int(long nb)
{
	char	value;
	int		counter;

	counter = 0;
	if (nb == 0)
		return (write(1, "0", 1));
	else if (nb > 9)
	{
		counter = counter + ft_print_unsigned_int(nb / 10);
		counter = counter + ft_print_unsigned_int(nb % 10);
	}
	else
	{
		value = (nb + '0');
		counter = counter + write (1, &value, 1);
	}
	return (counter);
}
