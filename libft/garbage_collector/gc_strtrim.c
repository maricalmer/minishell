/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 19:59:33 by tmurua            #+#    #+#             */
/*   Updated: 2024/12/13 21:01:46 by tmurua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

/*	same as ft_strtrim, but adds trimmed string to garbage collector t_list
	allocate (with malloc(3)) and return a copy of ’s1’ with chars specified in
	'set' removed from the beginning and the end */
char	*gc_strtrim(t_list **gc_head, const char *s1, const char *set)
{
	char	*trimmed;

	if (!s1 || !set)
		return (NULL);
	trimmed = ft_strtrim(s1, set);
	if (!trimmed)
		return (NULL);
	gc_add_ptr_to_list(gc_head, trimmed);
	return (trimmed);
}
