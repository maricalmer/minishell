/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 16:38:55 by tmurua            #+#    #+#             */
/*   Updated: 2024/11/29 16:40:30 by tmurua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

static int	int_len(long nbr);

/* same as ft_itoa, but adds new str to the garbage collector t_list */
char	*gc_itoa(t_list **gc_head, int n)
{
	int		len;
	int		i;
	char	*str;
	long	nbr;

	nbr = n;
	len = int_len(nbr);
	str = gc_calloc(gc_head, len + 1, sizeof(char));
	if (!str)
		return (NULL);
	i = len - 1;
	if (nbr == 0)
		str[0] = '0';
	if (nbr < 0)
	{
		str[0] = '-';
		nbr = -nbr;
	}
	while (nbr > 0)
	{
		str[i--] = (nbr % 10) + '0';
		nbr /= 10;
	}
	return (str);
}

static int	int_len(long nbr)
{
	int	count;

	count = 0;
	if (nbr <= 0)
		count = 1;
	while (nbr != 0)
	{
		nbr /= 10;
		count++;
	}
	return (count);
}
