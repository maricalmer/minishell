/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_free_all.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 10:19:38 by tmurua            #+#    #+#             */
/*   Updated: 2024/11/20 12:16:35 by tmurua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

/* free all nodes in garbage_collector t_list and all their content */
void	gc_free_all(t_list *gc_head)
{
	t_list	*gc_current_node;
	t_list	*gc_next_node;

	gc_current_node = gc_head;
	while (gc_current_node)
	{
		gc_next_node = gc_current_node->next;
		if (gc_current_node->content)
			free(gc_current_node->content);
		free(gc_current_node);
		gc_current_node = gc_next_node;
	}
}
