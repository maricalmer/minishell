/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operator_syntax.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maricalmer <maricalmer@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 16:09:33 by tmurua            #+#    #+#             */
/*   Updated: 2025/05/05 23:16:47 by maricalmer       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/* Utility functions for syntax validation of shell input.                    */
/*  - Skips whitespace and identifies operators like | and &&.                */
/*  - Validates correct use of pipe and AND operators in the middle of input. */
/*  - Ensures heredoc and control characters follow expected syntax.          */
/*  - Supports higher-level parsing logic with structural helpers.            */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*skip_whitespace_input(char *str)
{
	while (*str && ft_iswhitespace(*str))
		str++;
	return (str);
}

const char	*skip_operator_right(const char *str, const char **last_valid,
		int *offset)
{
	if (*str == '|')
	{
		*last_valid = str;
		str++;
		*offset = 1;
	}
	else if (*str == '&')
	{
		if (*(str + 1) != '\0' && *(str + 1) == '&')
		{
			*last_valid = str;
			str += 2;
			*offset = 2;
		}
		else
			str++;
	}
	else
		str++;
	return (str);
}

const char	*handle_pipe_middle(const char *str)
{
	if (*(str + 1) != '\0' && *(str + 1) == '|')
		str += 2;
	else
		str++;
	str = skip_whitespace_input((char *)str);
	if (*str == '\0')
		return (str);
	if (*str == '|' || *str == '&')
		return (NULL);
	return (str);
}

const char	*handle_and_middle(const char *str)
{
	if (*(str + 1) != '\0' && *(str + 1) == '&')
	{
		str += 2;
		str = skip_whitespace_input((char *)str);
		if (*str == '\0')
			return (str);
		if (*str == '|' || *str == '&')
			return (NULL);
	}
	else
		str++;
	return (str);
}

int	check_heredoc_char(char **str)
{
	*str += 2;
	*str = skip_whitespace_input(*str);
	if (!**str || (**str == '<' || **str == '>' || **str == '(' || **str == ')'
			|| **str == '&' || **str == '#' || **str == '-' || **str == '|'
			|| **str == '\0'))
		return (0);
	return (1);
}
