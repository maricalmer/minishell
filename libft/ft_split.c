/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/30 18:26:31 by tmurua            #+#    #+#             */
/*   Updated: 2023/12/05 17:45:59 by tmurua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_count_words(char const *s, char c);
static void	free_arr_strs(char **arr_strs, size_t i);
static char	**split(char const *s, char c, char **arr_strs, size_t i);

char	**ft_split(char const *s, char c)
{
	size_t	i;
	char	**arr_strs;

	i = 0;
	arr_strs = (char **)malloc((ft_count_words(s, c) + 1) * sizeof(char *));
	if (!s || !arr_strs)
		return (0);
	arr_strs = split(s, c, arr_strs, i);
	return (arr_strs);
}

static int	ft_count_words(char const *s, char c)
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

static void	free_arr_strs(char **arr_strs, size_t i)
{
	while (0 < i)
	{
		i--;
		free(arr_strs[i]);
	}
	free(arr_strs);
}

static char	**split(char const *s, char c, char **arr_strs, size_t i)
{
	int	word_len;

	while (*s)
	{
		while (*s == c)
			s++;
		if (*s)
		{
			if (!ft_strchr(s, c))
				word_len = ft_strlen(s);
			else
				word_len = ft_strchr(s, c) - s;
			arr_strs[i] = ft_substr(s, 0, word_len);
			if (arr_strs[i] == NULL)
			{
				free_arr_strs(arr_strs, i);
				return (NULL);
			}
			i++;
			s = s + word_len;
		}
	}
	arr_strs[i] = NULL;
	return (arr_strs);
}
