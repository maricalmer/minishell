/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 18:10:19 by tmurua            #+#    #+#             */
/*   Updated: 2024/12/18 00:10:19 by tmurua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/* handle entering and leaving quotes; return 1 if quote handled, 0 otherwise */
int	handle_quotes(char *str, int *in_quotes, char *quote_char)
{
	if (!*in_quotes && (*str == '\'' || *str == '"'))
	{
		*in_quotes = 1;
		*quote_char = *str;
		return (1);
	}
	if (*in_quotes && *str == *quote_char)
	{
		*in_quotes = 0;
		*quote_char = '\0';
		return (1);
	}
	return (0);
}

/* Validate that before '(' is valid or '(' itself */
int	valid_open_prevchar(char prev_char)
{
	return (prev_char == '\0' || prev_char == '|' || prev_char == '&'
		|| prev_char == '(');
}

/* Check closing parenthesis validity */
int	check_closing(const char **str)
{
	(*str)++;
	while (ft_iswhitespace(**str))
		(*str)++;
	if (**str != '\0' && **str != '|' && **str != '&' && **str != ')')
		return (0);
	return (1);
}

/* Check no-empty-parenthesis helper */
int	check_empty_parenthesis(const char **str)
{
	(*str)++;
	while (ft_iswhitespace(**str))
		(*str)++;
	if (**str == ')')
	{
		(*str)++;
		while (ft_iswhitespace(**str))
			(*str)++;
		if (**str == '\0')
			return (0);
	}
	return (1);
}

int	validate_no_empty_parenthesis(const char *str)
{
	while (*str != '\0')
	{
		if (*str == '(')
		{
			str++;
			while (ft_iswhitespace(*str))
				str++;
			if (*str == ')')
			{
				str++;
				while (ft_iswhitespace(*str))
					str++;
				if (*str == '\0')
					return (0);
			}
		}
		else
			str++;
	}
	return (1);
}
