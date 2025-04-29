/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_validation_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 16:09:33 by tmurua            #+#    #+#             */
/*   Updated: 2024/12/14 19:07:50 by tmurua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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
