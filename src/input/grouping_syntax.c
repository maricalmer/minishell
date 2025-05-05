/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grouping_syntax.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maricalmer <maricalmer@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 18:10:19 by tmurua            #+#    #+#             */
/*   Updated: 2025/05/05 23:18:02 by maricalmer       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/* Handles quote and parenthesis-related parsing rules in shell input.        */
/*  - Tracks and updates quote states for proper grouping.                    */
/*  - Validates characters before opening parentheses.                        */
/*  - Ensures closing parentheses follow valid syntax.                        */
/*  - Detects and rejects empty or invalid parenthesis usage.                 */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	valid_open_prevchar(char prev_char)
{
	return (prev_char == '\0' || prev_char == '|' || prev_char == '&'
		|| prev_char == '(');
}

int	check_closing(const char **str)
{
	(*str)++;
	while (ft_iswhitespace(**str))
		(*str)++;
	if (**str != '\0' && **str != '|' && **str != '&' && **str != ')')
		return (0);
	return (1);
}

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
