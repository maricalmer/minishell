/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_lexer_state_handler.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 21:05:22 by dlemaire          #+#    #+#             */
/*   Updated: 2025/05/08 20:07:07 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

void	init_lexer_for_test(t_lexer *lexer, const char *str)
{
	lexer->str = str;
	lexer->pos = 0;
	lexer->current_char = str[0];
	lexer->state = DEFAULT_STATE;
	lexer->command_expected = 1;
}

void	test_collect_token_simple_word(void)
{
	t_lexer lexer;
	t_minishell shell = {0};
	init_lexer_for_test(&lexer, "hello");

	char *token = collect_token(&lexer, &shell);

	CU_ASSERT_PTR_NOT_NULL(token);
	CU_ASSERT_STRING_EQUAL(token, "hello");
	free(token);
}

void	test_collect_token_with_single_quotes(void)
{
	t_lexer lexer;
	t_minishell shell = {0};
	init_lexer_for_test(&lexer, "'quoted'");

	char *token = collect_token(&lexer, &shell);

	CU_ASSERT_PTR_NOT_NULL(token);
	CU_ASSERT_STRING_EQUAL(token, "quoted");
	free(token);
}

void	test_collect_token_ends_on_whitespace(void)
{
	t_lexer lexer;
	t_minishell shell = {0};
	init_lexer_for_test(&lexer, "abc def");

	char *token = collect_token(&lexer, &shell);

	CU_ASSERT_PTR_NOT_NULL(token);
	CU_ASSERT_STRING_EQUAL(token, "abc");
	free(token);
}

void	test_handle_default_state_transition(void)
{
	t_lexer lexer;
	t_minishell shell = {0};
	char *buffer = gc_strdup(&shell.gc_head, "");

	lexer.str = "\"";
	lexer.current_char = '"';
	lexer.pos = 0;
	lexer.state = DEFAULT_STATE;

	int result = handle_default_state(&lexer, &buffer, &shell);

	CU_ASSERT_EQUAL(lexer.state, DOUBLE_QUOTE_STATE);
	CU_ASSERT_EQUAL(result, TOKEN_CONTINUE);
	free(buffer);
}

int	add_lexer_state_handler_tests(void)
{
    CU_pSuite suite = CU_add_suite("lexer_state_handler", 0, 0);
    
    if (!suite)
        return (1);
    if (!CU_add_test(suite, "test_collect_token_simple_word", test_collect_token_simple_word) ||
        !CU_add_test(suite, "test_collect_token_with_single_quotes", test_collect_token_with_single_quotes) ||
        !CU_add_test(suite, "test_collect_token_ends_on_whitespace", test_collect_token_ends_on_whitespace) ||
        !CU_add_test(suite, "test_handle_default_state_transition", test_handle_default_state_transition))
    {
        return (1);
    }
    return (0);
}