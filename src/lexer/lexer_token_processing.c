/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_token_processing.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maricalmer <maricalmer@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 07:08:16 by tmurua            #+#    #+#             */
/*   Updated: 2025/05/06 10:05:35 by maricalmer       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/* Entry point for lexical analysis of shell input.                           */
/*  - Initializes lexer state and associates it with the shell structure.     */
/*  - Parses the input into token structures, handling lexical errors.        */
/*  - Detects and manages unclosed quotes or invalid token sequences.         */
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
		cl_parenth_count = count_remove_trailing_parenth(new_token->value);
		if (*new_token->value != '\0')
			token_to_list(&(shell->tokens), new_token);
		add_closing_parentheses(cl_parenth_count, shell);
	}
	return (0);
}

void	skip_whitespace(t_lexer *lexer)
{
	while (lexer->current_char != '\0' && ft_iswhitespace(lexer->current_char))
		advance_lexer_char(lexer);
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

int	count_remove_trailing_parenth(char *value)
{
	int		count;
	size_t	len;

	count = 0;
	if (!value)
		return (0);
	len = ft_strlen(value);
	while (len > 0 && value[len - 1] == ')')
	{
		count++;
		value[len - 1] = '\0';
		len--;
	}
	return (count);
}

int	handle_unclosed_quotes(t_lexer *lexer, t_minishell *shell)
{
	if (lexer->state == SINGLE_QUOTE_STATE
		|| lexer->state == DOUBLE_QUOTE_STATE)
	{
		ft_putstr_fd("minishell: unexpected EOF while looking for matching `",
			2);
		shell->last_exit_status = 2;
		if (lexer->state == SINGLE_QUOTE_STATE)
			ft_putstr_fd("'", 2);
		else
			ft_putstr_fd("\"", 2);
		ft_putstr_fd("'\n", 2);
		shell->tokens = NULL;
		return (1);
	}
	return (0);
}
