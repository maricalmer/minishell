/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_hex.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 16:40:32 by tmurua            #+#    #+#             */
/*   Updated: 2024/08/06 13:38:15 by tmurua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_hex_len(unsigned long nb);
void	ft_put_hex(unsigned long nb, const char convert);

int	ft_print_hex(unsigned long nb, const char convert)
{
	if (nb == 0)
		return (write(1, "0", 1));
	else
		ft_put_hex(nb, convert);
	return (ft_hex_len(nb));
}

int	ft_hex_len(unsigned long nb)
{
	int	len;

	len = 0;
	while (nb != 0)
	{
		len++;
		nb = nb / 16;
	}
	return (len);
}

void	ft_put_hex(unsigned long nb, const char convert)
{
	if (nb >= 16)
	{
		ft_put_hex(nb / 16, convert);
		ft_put_hex(nb % 16, convert);
	}
	else
	{
		if (nb <= 9)
			ft_print_char((nb + '0'));
		else
		{
			if (convert == 'x')
				ft_print_char((nb - 10 + 'a'));
			if (convert == 'X')
				ft_print_char((nb - 10 + 'A'));
			if (convert == 'p')
				ft_print_char((nb - 10 + 'a'));
		}
	}
}
