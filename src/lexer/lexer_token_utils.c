/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_token_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maricalmer <maricalmer@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 17:02:08 by tmurua            #+#    #+#             */
/*   Updated: 2025/05/06 10:15:38 by maricalmer       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/* Utility functions for lexer and token list operations.                     */
/*  - Includes token counting, list management, and redirect updates.         */
/*  - Supports linked list manipulation (add, get last) for tokens.           */
/*  - Handles lexer character advancement and redirect-related token typing.  */
/*  - Aids higher-level lexer logic with modular, reusable helpers.           */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_tokens(t_token *tokens)
{
	int		count;
	t_token	*current;

	count = 0;
	current = tokens;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}

void	token_to_list(t_token **tokens, t_token *new)
{
	t_token	*tmp;

	if (!*tokens)
		*tokens = new;
	else
	{
		tmp = *tokens;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}

t_token	*get_last_token(t_token *lst)
{
	t_token	*tmp;

	if (!lst)
		return (NULL);
	tmp = lst;
	while (tmp->next)
		tmp = tmp->next;
	return (tmp);
}

void	update_redirect_tokens(t_token *tokens, t_minishell *shell)
{
	while (tokens)
	{
		if ((tokens->type == TOKEN_REDIRECT_IN
				|| tokens->type == TOKEN_REDIRECT_OUT
				|| tokens->type == TOKEN_REDIRECT_APPEND)
			&& tokens->next)
			tokens->next->type = TOKEN_FILENAME;
		if (tokens->type == TOKEN_HEREDOC)
		{
			if (tokens->next)
				tokens->next->type = TOKEN_HEREDOC_DELIMITER;
			else
			{
				ft_putstr_fd("minishell: syntax error near unexpected token\n",
					2);
				shell->last_exit_status = 2;
			}
		}
		tokens = tokens->next;
	}
}

void	advance_lexer_char(t_lexer *lexer)
{
	lexer->pos++;
	if (lexer->pos < ft_strlen(lexer->str))
		lexer->current_char = lexer->str[lexer->pos];
	else
		lexer->current_char = '\0';
}
