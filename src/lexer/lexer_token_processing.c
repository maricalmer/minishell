/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_token_processing.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 07:08:16 by tmurua            #+#    #+#             */
/*   Updated: 2025/05/08 22:50:30 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/* Processes raw input into tokens, handling whitespace and token extraction. */
/* Strips unmatched trailing parentheses from tokens, except those inside     */
/* quotes. Restores stripped parentheses later to preserve grouping syntax.   */
/* Handles quoted strings and compound tokens within shell input.             */
/* Ensures proper token list construction for parser consumption.             */
/* Avoids removing parentheses that are part of valid quoted expressions.     */
/* Critical for correct interpretation of command groups and pipelines.       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	process_lexer_tokens(t_lexer *lexer, t_minishell *shell)
{
	t_token	*new_token;
	int		cl_parenth_count;

	while (lexer->current_char != '\0')
	{
		skip_whitespace(lexer);
		new_token = get_next_token(lexer, shell);
		if (!new_token || new_token->type == TOKEN_INVALID)
			return (1);
		cl_parenth_count = count_remove_trailing_parenth(lexer,
				new_token->value);
		if (*new_token->value != '\0')
			token_to_list(&(shell->tokens), new_token);
		add_closing_parentheses(cl_parenth_count, shell);
	}
	return (0);
}

void	add_closing_parentheses(int count, t_minishell *shell)
{
	t_token	*cl_parenthesis_token;

	while (count--)
	{
		cl_parenthesis_token = create_token(TOKEN_CL_PARENTHESIS, ")", shell);
		if (!cl_parenthesis_token)
			continue ;
		token_to_list(&(shell->tokens), cl_parenthesis_token);
	}
}

int	get_token_length_in_original_input(t_lexer *lexer)
{
	const char	*token_str;
	int			token_len;

	token_len = 0;
	token_str = lexer->str + lexer->pos - 1;
	while (token_str[token_len] && token_str[token_len] != ' '
		&& token_str[token_len] != '|' && token_str[token_len] != ';')
		token_len++;
	return (token_len);
}

void	count_removable_parentheses_in_token(t_lexer *lexer,
	int *removable_count)
{
	int		token_len;
	size_t	i;
	char	c;
	int		in_quotes;

	token_len = get_token_length_in_original_input(lexer);
	i = lexer->pos - 1 + token_len - 1;
	in_quotes = 0;
	while (i >= lexer->pos - 1)
	{
		c = lexer->str[i];
		if (c == '"')
			in_quotes = !in_quotes;
		else if (!in_quotes && c == ')')
			(*removable_count)++;
		else if (!in_quotes && c != ')')
			break ;
		i--;
	}
}

int	count_remove_trailing_parenth(t_lexer *lexer, char *value)
{
	int		count;
	size_t	val_len;
	int		removable_count;

	if (!value || !lexer || !lexer->str)
		return (0);
	removable_count = 0;
	count_removable_parentheses_in_token(lexer, &removable_count);
	count = 0;
	val_len = ft_strlen(value);
	while (val_len > 0 && removable_count > 0 && value[val_len - 1] == ')')
	{
		value[val_len - 1] = '\0';
		val_len--;
		count++;
		removable_count--;
	}
	return (count);
}
