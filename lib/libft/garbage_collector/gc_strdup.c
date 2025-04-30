/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 10:28:48 by tmurua            #+#    #+#             */
/*   Updated: 2024/11/20 12:16:40 by tmurua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

/* same as ft_strdup, but adds dup(dest) string to garbage collector t_list */
char	*gc_strdup(t_list **gc_head, const char *s1)
{
	char	*gc_dest;
	size_t	len;

	len = ft_strlen(s1);
	gc_dest = gc_calloc(gc_head, len + 1, sizeof(char));
	if (!gc_dest)
		return (NULL);
	ft_strlcpy(gc_dest, s1, len + 1);
	return (gc_dest);
}
