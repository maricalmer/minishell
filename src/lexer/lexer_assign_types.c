/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_assign_types.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 18:06:47 by tmurua            #+#    #+#             */
/*   Updated: 2024/12/17 00:59:01 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/* check if current lexer char is a special token character */
int	is_special_character(t_lexer *lexer)
{
	char	c;
	char	next_c;

	c = lexer->current_char;
	next_c = lexer->str[lexer->pos + 1];
	if (c == '|' || c == '<' || c == '>' || c == '&' || c == '!' || c == '(')
		return (1);
	if ((c == '&' && next_c == '&') || (c == '|' && next_c == '|') || (c == '<'
			&& next_c == '<') || (c == '>' && next_c == '>'))
		return (1);
	return (0);
}

/*	collect special char token, assign its type, update lexer->command_expected;
	return token type */
t_token_type	handle_special_char_token(t_lexer *lexer, char **value,
		t_minishell *shell)
{
	t_token_type	type;

	*value = collect_special_character(lexer, shell);
	if (!*value)
		return (TOKEN_INVALID);
	type = get_special_character_token_type(*value);
	if (type == TOKEN_PIPE || type == TOKEN_AND || type == TOKEN_OR
		|| type == TOKEN_OP_PARENTHESIS)
		lexer->command_expected = 1;
	return (type);
}

/* set *special_char_str with maximum of 2 chars + null terminator */
char	*collect_special_character(t_lexer *lexer, t_minishell *shell)
{
	int		i;
	char	special_char_str[3];

	i = 0;
	special_char_str[i++] = lexer->current_char;
	if ((lexer->current_char == '&' && lexer->str[lexer->pos + 1] == '&')
		|| (lexer->current_char == '|' && lexer->str[lexer->pos + 1] == '|')
		|| (lexer->current_char == '<' && lexer->str[lexer->pos + 1] == '<')
		|| (lexer->current_char == '>' && lexer->str[lexer->pos + 1] == '>'))
	{
		lexer->pos++;
		lexer->current_char = lexer->str[lexer->pos];
		special_char_str[i++] = lexer->current_char;
	}
	special_char_str[i] = '\0';
	advance_lexer_char(lexer);
	return (gc_strdup(&shell->gc_head, special_char_str));
}

/* single & is not supported */
t_token_type	get_special_character_token_type(char *value)
{
	if (ft_strncmp(value, "|", 2) == 0)
		return (TOKEN_PIPE);
	else if (ft_strncmp(value, "||", 3) == 0)
		return (TOKEN_OR);
	else if (ft_strncmp(value, "&", 2) == 0)
		return (TOKEN_INVALID);
	else if (ft_strncmp(value, "&&", 3) == 0)
		return (TOKEN_AND);
	else if (ft_strncmp(value, "<", 2) == 0)
		return (TOKEN_REDIRECT_IN);
	else if (ft_strncmp(value, ">", 2) == 0)
		return (TOKEN_REDIRECT_OUT);
	else if (ft_strncmp(value, "<<", 3) == 0)
		return (TOKEN_HEREDOC);
	else if (ft_strncmp(value, ">>", 3) == 0)
		return (TOKEN_REDIRECT_APPEND);
	else if (ft_strncmp(value, "(", 2) == 0)
		return (TOKEN_OP_PARENTHESIS);
	else if (ft_strncmp(value, ")", 2) == 0)
		return (TOKEN_CL_PARENTHESIS);
	else
		return (TOKEN_INVALID);
}

/*	collect regular token, assign its type based on whether a cmd is expected,
	and updates the lexer state; return token type */
t_token_type	handle_regular_token(t_lexer *lexer, char **value,
		t_minishell *shell)
{
	t_token_type	type;

	*value = collect_token(lexer, shell);
	if (!*value)
		return (TOKEN_INVALID);
	if ((*value)[0] == '\0')
		return (TOKEN_INVALID);
	if (shell->tokens && get_last_token(shell->tokens)->type == TOKEN_HEREDOC)
		type = TOKEN_HEREDOC_DELIMITER;
	else if (shell->tokens && (
			get_last_token(shell->tokens)->type == TOKEN_REDIRECT_IN
			|| get_last_token(shell->tokens)->type == TOKEN_REDIRECT_OUT
			|| get_last_token(shell->tokens)->type == TOKEN_REDIRECT_APPEND))
		type = TOKEN_FILENAME;
	else if (lexer->command_expected == 1)
	{
		if (is_builtin_command(*value))
			type = TOKEN_BUILTIN_CMD;
		else
			type = TOKEN_EXTERN_CMD;
		lexer->command_expected = 0;
	}
	else
		type = TOKEN_ARGUMENT;
	return (type);
}
