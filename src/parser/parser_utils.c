/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maricalmer <maricalmer@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 10:11:39 by tmurua            #+#    #+#             */
/*   Updated: 2025/05/06 10:20:51 by maricalmer       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/* Utility functions for parser logic and AST construction.                   */
/*  - Determines if a token is a statement-level operator (||, &&, |).        */
/*  - Provides operator precedence levels for proper AST structure.           */
/*  - Supports linked list management of token sequences.                     */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_statement_delimiter(int type)
{
	if (type == TOKEN_OR || type == TOKEN_AND || type == TOKEN_PIPE)
		return (1);
	return (0);
}

int	get_precedence_lvl(int type)
{
	if (type == TOKEN_OR || type == TOKEN_AND)
		return (0);
	return (1);
}

void	ft_tkadd_back(t_token **tokens, t_token *new)
{
	t_token	*tmp;

	if (tokens)
	{
		tmp = *tokens;
		if (*tokens == NULL)
			*tokens = new;
		else
		{
			while (tmp->next)
				tmp = tmp->next;
			tmp->next = new;
		}
	}
}
