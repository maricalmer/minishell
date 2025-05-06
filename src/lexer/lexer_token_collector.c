/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_token_collector.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maricalmer <maricalmer@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 16:42:16 by tmurua            #+#    #+#             */
/*   Updated: 2025/05/06 10:11:00 by maricalmer       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/* Manages lexer behavior based on parsing state (default, quotes, etc.).     */
/*  - Collects characters into tokens according to current lexer state.       */
/*  - Handles state transitions for quotes and variable expansion.            */
/*  - Terminates or continues token collection based on special characters.   */
/*  - Ensures safe token construction via state-specific handlers.            */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*collect_token(t_lexer *lexer, t_minishell *shell)
{
	char	*buffer;
	int		status;

	buffer = gc_strdup(&shell->gc_head, "");
	if (!buffer)
		return (NULL);
	while (lexer->current_char != '\0')
	{
		if (lexer->state == DEFAULT_STATE)
			status = handle_default_state(lexer, &buffer, shell);
		else if (lexer->state == SINGLE_QUOTE_STATE)
			status = handle_single_quote_state(lexer, &buffer, shell);
		else if (lexer->state == DOUBLE_QUOTE_STATE)
			status = handle_double_quote_state(lexer, &buffer, shell);
		if (status == TOKEN_COMPLETE)
			break ;
		else if (status == TOKEN_ERROR)
			return (NULL);
	}
	return (buffer);
}

int	handle_default_state(t_lexer *lexer, char **buffer, t_minishell *shell)
{
	if (ft_iswhitespace(lexer->current_char))
		return (TOKEN_COMPLETE);
	else if (lexer->current_char == '\'')
	{
		lexer->state = SINGLE_QUOTE_STATE;
		advance_lexer_char(lexer);
	}
	else if (lexer->current_char == '"')
	{
		lexer->state = DOUBLE_QUOTE_STATE;
		advance_lexer_char(lexer);
	}
	else if (lexer->current_char == '$')
		return (handle_dollar_sign(lexer, buffer, shell));
	else if (is_special_character(lexer))
		return (TOKEN_COMPLETE);
	else
	{
		if (!advance_and_append(lexer, buffer, shell))
			return (TOKEN_ERROR);
	}
	return (TOKEN_CONTINUE);
}

int	handle_single_quote_state(t_lexer *lexer, char **buffer, t_minishell *shell)
{
	if (lexer->current_char == '\'')
	{
		lexer->state = DEFAULT_STATE;
		advance_lexer_char(lexer);
	}
	else
	{
		if (!advance_and_append(lexer, buffer, shell))
			return (TOKEN_ERROR);
	}
	return (TOKEN_CONTINUE);
}

int	handle_double_quote_state(t_lexer *lexer, char **buffer, t_minishell *shell)
{
	if (lexer->current_char == '"')
	{
		lexer->state = DEFAULT_STATE;
		advance_lexer_char(lexer);
	}
	else if (lexer->current_char == '$')
		return (handle_dollar_sign(lexer, buffer, shell));
	else
	{
		if (!advance_and_append(lexer, buffer, shell))
			return (TOKEN_ERROR);
	}
	return (TOKEN_CONTINUE);
}
