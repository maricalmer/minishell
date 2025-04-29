/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 07:08:16 by tmurua            #+#    #+#             */
/*   Updated: 2024/12/17 07:09:29 by tmurua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/* loop over input chars, creating tokens until EOF or error */
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

/*	skip all leading whitespace characters in the input */
void	skip_whitespace(t_lexer *lexer)
{
	while (lexer->current_char != '\0' && ft_iswhitespace(lexer->current_char))
		advance_lexer_char(lexer);
}

/* create and append ')' tokens to shell's token list based on the count */
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

/* count and remove trailing ')' chars from token value */
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

/* check for unclosed quotes and print error if found */
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
