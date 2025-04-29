/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_state_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 21:51:08 by tmurua            #+#    #+#             */
/*   Updated: 2024/12/15 22:04:23 by tmurua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/* function still has too many lines of code (29) */
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

/*	current_char_str: temp str to hold current_char as a null-terminated str
	join current buffer with current_char_str at the end, move lexer forward */
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
