/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_add_ptr_to_list.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 10:21:07 by tmurua            #+#    #+#             */
/*   Updated: 2024/11/20 12:18:14 by tmurua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

/* add new node to the end of the garbage_collector t_list */
void	gc_add_ptr_to_list(t_list **gc_head, void *new_ptr)
{
	t_list	*gc_new_node;
	t_list	*gc_tmp;

	gc_new_node = ft_calloc(1, sizeof(t_list));
	if (!gc_new_node)
	{
		ft_putstr_fd("Error: Failed to allocate memory\n", 2);
		return ;
	}
	gc_new_node->content = new_ptr;
	gc_new_node->next = NULL;
	if (!(*gc_head))
	{
		(*gc_head) = gc_new_node;
		return ;
	}
	gc_tmp = *gc_head;
	while (gc_tmp->next)
		gc_tmp = gc_tmp->next;
	gc_tmp->next = gc_new_node;
}
