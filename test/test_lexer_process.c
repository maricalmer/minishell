/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_lexer_process.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 21:05:22 by dlemaire          #+#    #+#             */
/*   Updated: 2025/04/29 21:41:55 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

t_token *mock_token = NULL;
int mock_get_next_token_called = 0;

void test_skip_whitespace(void) {
	t_lexer lexer = { .str = "   abc", .pos = 0, .current_char = ' ', .state = 0 };
	advance_lexer_char(&lexer); // set next char
	lexer.current_char = lexer.str[lexer.pos];
	skip_whitespace(&lexer);
	CU_ASSERT_EQUAL(lexer.str[lexer.pos], 'a');
}

void test_count_remove_trailing_parenth(void) {
	char value[] = "echo)))";
	int count = count_remove_trailing_parenth(value);
	CU_ASSERT_EQUAL(count, 3);
	CU_ASSERT_STRING_EQUAL(value, "echo");
}

void test_add_closing_parentheses(void) {
	t_minishell shell = {0};
	add_closing_parentheses(2, &shell);
	CU_ASSERT_PTR_NOT_NULL(shell.tokens);
	CU_ASSERT_EQUAL(shell.tokens->type, TOKEN_CL_PARENTHESIS);
	CU_ASSERT_PTR_NOT_NULL(shell.tokens->next);
	CU_ASSERT_EQUAL(shell.tokens->next->type, TOKEN_CL_PARENTHESIS);
}

void test_handle_unclosed_quotes_single(void) {
	t_minishell shell = {0};
	t_lexer lexer = {.state = SINGLE_QUOTE_STATE};
	int result = handle_unclosed_quotes(&lexer, &shell);
	CU_ASSERT_EQUAL(result, 1);
	CU_ASSERT_EQUAL(shell.last_exit_status, 2);
}

void test_handle_unclosed_quotes_double(void) {
	t_minishell shell = {0};
	t_lexer lexer = {.state = DOUBLE_QUOTE_STATE};
	int result = handle_unclosed_quotes(&lexer, &shell);
	CU_ASSERT_EQUAL(result, 1);
	CU_ASSERT_EQUAL(shell.last_exit_status, 2);
}

void test_process_lexer_tokens_valid_token(void) {
	t_minishell shell = {0};
	t_lexer lexer = { .str = "echo)", .pos = 0, .current_char = 'e' };
	t_token token = {.type = TOKEN_ARGUMENT, .value = strdup("echo)"), .next = NULL};
	mock_token = &token;

	int result = process_lexer_tokens(&lexer, &shell);

	CU_ASSERT_EQUAL(result, 0);
	CU_ASSERT_PTR_NOT_NULL(shell.tokens);
	CU_ASSERT_STRING_EQUAL(shell.tokens->value, "echo");
	CU_ASSERT_PTR_NOT_NULL(shell.tokens->next);
	CU_ASSERT_STRING_EQUAL(shell.tokens->next->value, ")");
}

int add_lexer_process_tests(void)
{
    CU_pSuite suite = CU_add_suite("lexer_process", 0, 0);
    
    if (!suite)
        return (1);
    
    if (!CU_add_test(suite, "test_skip_whitespace", test_skip_whitespace) ||
        !CU_add_test(suite, "test_count_remove_trailing_parenth", test_count_remove_trailing_parenth) ||
        !CU_add_test(suite, "test_add_closing_parentheses", test_add_closing_parentheses) ||
        !CU_add_test(suite, "test_handle_unclosed_quotes_single", test_handle_unclosed_quotes_single) ||
        !CU_add_test(suite, "test_handle_unclosed_quotes_double", test_handle_unclosed_quotes_double) ||
        !CU_add_test(suite, "test_process_lexer_tokens_valid_token", test_process_lexer_tokens_valid_token))
    {
        return (1);
    }
    
    return (0);
}