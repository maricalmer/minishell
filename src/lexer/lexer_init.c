/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maricalmer <maricalmer@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 17:35:59 by dlemaire          #+#    #+#             */
/*   Updated: 2025/05/06 10:02:19 by maricalmer       ###   ########.fr       */
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

void	run_lexer(char *str, t_minishell *shell)
{
	t_lexer	lexer;

	initialize_lexer_and_tokens(str, &lexer, shell);
	if (process_lexer_tokens(&lexer, shell))
		return ;
	if (handle_unclosed_quotes(&lexer, shell))
		return ;
}

void	initialize_lexer_and_tokens(char *str, t_lexer *lexer,
		t_minishell *shell)
{
	*lexer = init_lexer(str);
	shell->tokens = NULL;
}

t_lexer	init_lexer(const char *arg)
{
	t_lexer	lexer;

	lexer.str = arg;
	lexer.pos = 0;
	lexer.current_char = arg[0];
	lexer.state = DEFAULT_STATE;
	lexer.command_expected = 1;
	return (lexer);
}
