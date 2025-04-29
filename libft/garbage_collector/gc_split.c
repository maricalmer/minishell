/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 11:26:54 by tmurua            #+#    #+#             */
/*   Updated: 2024/11/27 12:52:53 by tmurua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

static int	gc_count_words(const char *s, char c);
static char	**gc_allocate_array(t_list **gc_head, size_t word_count);
static int	gc_extract_words(t_list **gc_head, char const *s, char c,
				char **arr_strs);
static void	gc_free_arr_strs(char **arr_strs, size_t i);

char	**gc_split(t_list **gc_head, char const *s, char c)
{
	char	**arr_strs;
	size_t	word_count;

	if (!s)
		return (NULL);
	word_count = gc_count_words(s, c);
	arr_strs = gc_allocate_array(gc_head, word_count);
	if (!arr_strs)
		return (NULL);
	if (!gc_extract_words(gc_head, s, c, arr_strs))
		return (NULL);
	return (arr_strs);
}

/* count number of words in string 's' separated by delimiter 'c' */
static int	gc_count_words(const char *s, char c)
{
	int	count;

	count = 0;
	while (*s)
	{
		while (*s == c)
			s++;
		if (*s)
			count++;
		while (*s && *s != c)
			s++;
	}
	return (count);
}

/*	allocate mem using gc_calloc & track allocations with garbage collector */
static char	**gc_allocate_array(t_list **gc_head, size_t word_count)
{
	char	**arr_strs;

	arr_strs = gc_calloc(gc_head, word_count + 1, sizeof(char *));
	if (!arr_strs)
		return (NULL);
	return (arr_strs);
}

/*	extract words from the string and store them in the array */
static int	gc_extract_words(t_list **gc_head, char const *s, char c,
		char **arr_strs)
{
	size_t	i;
	int		word_len;

	i = 0;
	while (*s)
	{
		while (*s == c)
			s++;
		if (*s)
		{
			word_len = ft_strchr(s, c) - s;
			if (!ft_strchr(s, c))
				word_len = ft_strlen(s);
			arr_strs[i] = gc_substr(gc_head, s, 0, word_len);
			if (arr_strs[i] == NULL)
			{
				gc_free_arr_strs(arr_strs, i);
				return (0);
			}
			i++;
			s += word_len;
		}
	}
	arr_strs[i] = NULL;
	return (1);
}

/*	free array of strings up to index 'i' in case of an allocation failure */
static void	gc_free_arr_strs(char **arr_strs, size_t count)
{
	size_t	i;

	i = 0;
	while (i < count)
	{
		free(arr_strs[i]);
		i++;
	}
	free(arr_strs);
}
