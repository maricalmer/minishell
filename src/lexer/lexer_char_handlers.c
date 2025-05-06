/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_char_handlers.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maricalmer <maricalmer@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 21:51:08 by tmurua            #+#    #+#             */
/*   Updated: 2025/05/06 10:10:57 by maricalmer       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/* Provides utility functions for lexer state handling and token building.    */
/*  - Handles `$` for variable expansion or literal inclusion.                */
/*  - Appends current character to token buffer and advances lexer position.  */
/*  - Supports state-specific logic used during token collection.             */
/*  - Ensures memory safety via garbage-collected string operations.          */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_dollar_sign(t_lexer *lexer, char **buffer, t_minishell *shell)
{
	size_t	str_len;
	char	next_char;

	str_len = ft_strlen(lexer->str);
	if (lexer->pos + 1 < str_len)
		next_char = lexer->str[lexer->pos + 1];
	else
		next_char = '\0';
	if (next_char == '?' || ft_isalnum(next_char) || next_char == '_')
	{
		if (!handle_variable_expansion(lexer, buffer, shell))
			return (TOKEN_ERROR);
	}
	else
	{
		advance_lexer_char(lexer);
		*buffer = gc_strjoin(&shell->gc_head, *buffer, "$");
		if (!*buffer)
			return (TOKEN_ERROR);
	}
	return (TOKEN_CONTINUE);
}

int	advance_and_append(t_lexer *lexer, char **buffer, t_minishell *shell)
{
	char	current_char_str[2];

	current_char_str[0] = lexer->current_char;
	current_char_str[1] = '\0';
	*buffer = gc_strjoin(&shell->gc_head, *buffer, current_char_str);
	if (!*buffer)
		return (0);
	advance_lexer_char(lexer);
	return (1);
}
