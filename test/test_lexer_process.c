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

t_token *create_mock_token(const char *value, int type);
void reset_shell(t_minishell *shell);
t_lexer init_lexer(const char *input);

void assert_token_sequence(t_token *head, const char **expected_values, int expected_count) {
	int i = 0;
	while (head && i < expected_count) {
		CU_ASSERT_STRING_EQUAL(head->value, expected_values[i]);
		head = head->next;
		i++;
	}
	CU_ASSERT_EQUAL(i, expected_count);
}

// Test 1: Quoted parentheses should not be removed
void test_quoted_parentheses(void) {
	t_minishell shell = {0};
	t_lexer lexer = init_lexer("echo \"hello)\"");
	CU_ASSERT_EQUAL(process_lexer_tokens(&lexer, &shell), 0);

	const char *expected[] = { "echo", "hello)" };
	assert_token_sequence(shell.tokens, expected, 2);
}

// Test 2: Unquoted trailing parentheses should be removed
void test_unquoted_parentheses_removal(void) {
	t_minishell shell = {0};
	t_lexer lexer = init_lexer("(echo hello)");
	CU_ASSERT_EQUAL(process_lexer_tokens(&lexer, &shell), 0);

	const char *expected[] = { "(", "echo", "hello", ")" };
	assert_token_sequence(shell.tokens, expected, 4);
}

// Test 3: Balanced parentheses inside quotes preserved
void test_balanced_parentheses_in_quotes(void) {
	t_minishell shell = {0};
	t_lexer lexer = init_lexer("echo \"(hello world)\"");
	CU_ASSERT_EQUAL(process_lexer_tokens(&lexer, &shell), 0);

	const char *expected[] = { "echo", "(hello world)" };
	assert_token_sequence(shell.tokens, expected, 2);
}

int add_lexer_process_tests(void)
{
    CU_pSuite suite = CU_add_suite("lexer_process", 0, 0);
    
    if (!suite)
        return (1);
    
    if (!CU_add_test(suite, "test_quoted_parentheses", test_quoted_parentheses) ||
        !CU_add_test(suite, "test_unquoted_parentheses_removal", test_unquoted_parentheses_removal) ||
        !CU_add_test(suite, "test_balanced_parentheses_in_quotes", test_balanced_parentheses_in_quotes))
    {
        return (1);
    }
    
    return (0);
}