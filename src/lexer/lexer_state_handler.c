/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_state_handler.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 16:42:16 by tmurua            #+#    #+#             */
/*   Updated: 2024/12/15 21:52:16 by tmurua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*	collects the next token from the lexer based on its current state */
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

/*	if whitespace found, end current token; if ' or " found, change STATE;
	if $ found, handle var expansion; if special char found, end current token;
	otherwise advance_and_append each char */
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

/* if ' found, change to DEFAULT_STATE; advance_and_append all chars */
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

/* if " found, change to DEFAULT_STATE; handle $; or advance_and_append*/
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
