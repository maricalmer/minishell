/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 10:34:44 by tmurua            #+#    #+#             */
/*   Updated: 2024/11/20 12:16:38 by tmurua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

/* same as ft_strjoin, but adds join result to the garbage collector t_list */
char	*gc_strjoin(t_list **gc_head, const char *s1, const char *s2)
{
	char	*gc_join;
	size_t	len_s1;
	size_t	len_s2;

	if (!s1 || !s2)
		return (NULL);
	len_s1 = ft_strlen(s1);
	len_s2 = ft_strlen(s2);
	gc_join = gc_calloc(gc_head, len_s1 + len_s2 + 1, sizeof(char));
	if (!gc_join)
		return (NULL);
	ft_strlcpy(gc_join, s1, len_s1 + 1);
	ft_strlcat(gc_join, s2, len_s1 + len_s2 + 1);
	return (gc_join);
}
