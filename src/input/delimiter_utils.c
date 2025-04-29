/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delimiter_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 18:16:30 by tmurua            #+#    #+#             */
/*   Updated: 2024/12/17 06:10:40 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/* check if left delimiter is valid */
int	check_left_char(const char *str)
{
	if (*str == '|')
		return (0);
	if (*str == '&' && *(str + 1) == '&')
		return (0);
	return (1);
}

/* check if right delimiter end is correct */
int	check_right_end(const char *last_valid, int offset)
{
	const char	*check_pos;

	if (last_valid)
	{
		check_pos = last_valid + offset;
		check_pos = skip_whitespace_input((char *)check_pos);
		if (*check_pos == '\0')
			return (0);
	}
	return (1);
}

const char	*process_delimiter_char(const char *str, int *in_quotes,
		char *quote_char)
{
	if (handle_quotes((char *)str, in_quotes, quote_char))
		return (str + 1);
	if (*in_quotes)
		return (str + 1);
	if (*str == '|')
		return (handle_pipe_middle(str));
	else if (*str == '&')
		return (handle_and_middle(str));
	return (str + 1);
}

const char	*process_opening_char(const char *str, int *in_quotes,
		char *quote_char, char *prev_char)
{
	if (handle_quotes((char *)str, in_quotes, quote_char))
		return (str + 1);
	if (*in_quotes)
	{
		*prev_char = *str;
		return (str + 1);
	}
	if (*str == '(' && !valid_open_prevchar(*prev_char))
		return (NULL);
	*prev_char = *str;
	return (str + 1);
}
