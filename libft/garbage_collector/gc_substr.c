/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 12:25:18 by tmurua            #+#    #+#             */
/*   Updated: 2024/11/27 12:27:24 by tmurua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

/* same as ft_substr, but mem alloc uses gc_calloc to add alloc substr to gc */
char	*gc_substr(t_list **gc_head, const char *s, unsigned int start,
		size_t len)
{
	char	*gc_sub;
	size_t	s_len;

	if (!s)
		return (NULL);
	s_len = ft_strlen(s);
	if (start >= s_len)
		return (gc_calloc(gc_head, 1, sizeof(char)));
	if (len > s_len - start)
		len = s_len - start;
	gc_sub = gc_calloc(gc_head, len + 1, sizeof(char));
	if (!gc_sub)
		return (NULL);
	ft_strlcpy(gc_sub, s + start, len + 1);
	return (gc_sub);
}
