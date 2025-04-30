/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 20:41:12 by tmurua            #+#    #+#             */
/*   Updated: 2023/11/20 20:55:43 by tmurua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	unsigned char	*dest;
	unsigned char	*source;

	if (src == NULL && dst == NULL)
		return (dst);
	dest = (unsigned char *)dst;
	source = (unsigned char *)src;
	while (n > 0)
	{
		*(dest++) = *(source++);
		n--;
	}
	return (dst);
}
