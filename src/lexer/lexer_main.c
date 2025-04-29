/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmurua <tmurua@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 17:35:59 by dlemaire          #+#    #+#             */
/*   Updated: 2024/12/17 07:14:19 by tmurua           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	run_lexer(char *str, t_minishell *shell)
{
	t_lexer	lexer;

	initialize_lexer_and_tokens(str, &lexer, shell);
	if (process_lexer_tokens(&lexer, shell))
		return ;
	if (handle_unclosed_quotes(&lexer, shell))
		return ;
}

/* init lexer and set shell tokens and current token pointers */
void	initialize_lexer_and_tokens(char *str, t_lexer *lexer,
		t_minishell *shell)
{
	*lexer = init_lexer(str);
	shell->tokens = NULL;
}

/*	init lexer with input str, set pos, state, and establish cmd is expected */
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
