/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 17:19:38 by tmurua            #+#    #+#             */
/*   Updated: 2024/11/20 19:39:30 by tmurua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

/* allocate memory for an array of count elements of size bytes each and adds
the allocated memory to the linked list by calling gc_add_to_list. */
void	*gc_calloc(t_list **gc_head, size_t count, size_t size)
{
	void	*gc_ptr;

	gc_ptr = ft_calloc(count, size);
	if (!gc_ptr)
	{
		ft_putstr_fd("Error: Failed to allocate memory\n", 2);
		return (NULL);
	}
	gc_add_ptr_to_list(gc_head, gc_ptr);
	return (gc_ptr);
}
