/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_token_factory.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maricalmer <maricalmer@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 23:48:14 by tmurua            #+#    #+#             */
/*   Updated: 2025/05/06 10:13:21 by maricalmer       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/* Handles creation of the next token in the lexer stream.                    */
/*  - Differentiates between special and regular tokens.                      */
/*  - Skips whitespace and recursively skips empty invalid tokens.            */
/*  - Wraps token construction using type, value, and garbage-collected heap. */
/*  - Ensures memory-safe token creation for shell parsing.                   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*get_next_token(t_lexer *lexer, t_minishell *shell)
{
	char			*value;
	t_token_type	type;
	t_token			*new_token;

	skip_whitespace(lexer);
	if (lexer->current_char == '\0')
		return (NULL);
	if (is_special_character(lexer))
		type = handle_special_char_token(lexer, &value, shell);
	else
		type = handle_regular_token(lexer, &value, shell);
	if (type == TOKEN_INVALID && value && value[0] == '\0')
		return (get_next_token(lexer, shell));
	if (type == TOKEN_INVALID)
		return (NULL);
	new_token = create_token(type, value, shell);
	return (new_token);
}

t_token	*create_token(t_token_type type, char *value, t_minishell *shell)
{
	t_token	*token;

	token = gc_calloc(&shell->gc_head, 1, sizeof(t_token));
	if (!token)
	{
		perror("minishell: create_token");
		gc_free_all(shell->gc_head);
		exit(EXIT_FAILURE);
	}
	token->type = type;
	token->value = value;
	token->next = NULL;
	return (token);
}
