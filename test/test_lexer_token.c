/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_lexer_token.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 21:05:22 by dlemaire          #+#    #+#             */
/*   Updated: 2025/04/29 21:41:55 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

// Helper to init shell + lexer
static void init_shell_and_lexer(t_minishell *shell, t_lexer *lexer, char *input) {
	shell->gc_head = NULL;
	shell->tokens = NULL;
	*lexer = init_lexer(input);
}

// === TESTS ===

void test_get_next_token_regular_success(void) {
	t_minishell shell;
	t_lexer lexer;
	init_shell_and_lexer(&shell, &lexer, "ls");

	t_token *token = get_next_token(&lexer, &shell);
	CU_ASSERT_PTR_NOT_NULL(token);
	CU_ASSERT_EQUAL(token->type, TOKEN_EXTERN_CMD);
	CU_ASSERT_STRING_EQUAL(token->value, "ls");
}

void test_get_next_token_special_success(void) {
	t_minishell shell;
	t_lexer lexer;
	init_shell_and_lexer(&shell, &lexer, "|");

	t_token *token = get_next_token(&lexer, &shell);
	CU_ASSERT_PTR_NOT_NULL(token);
	CU_ASSERT_EQUAL(token->type, TOKEN_PIPE);
	CU_ASSERT_STRING_EQUAL(token->value, "|");
}

void test_get_next_token_invalid_empty(void) {
	t_minishell shell;
	t_lexer lexer;
	init_shell_and_lexer(&shell, &lexer, "   ");

	t_token *token = get_next_token(&lexer, &shell);
	CU_ASSERT_PTR_NULL(token);
}

int add_lexer_token_tests(void)
{
    CU_pSuite suite = CU_add_suite("lexer_token", 0, 0);
    
    if (!suite)
        return (1);
    
    if (!CU_add_test(suite, "test_get_next_token_regular_success", test_get_next_token_regular_success) ||
        !CU_add_test(suite, "test_get_next_token_special_success", test_get_next_token_special_success) ||
        !CU_add_test(suite, "test_get_next_token_invalid_empty", test_get_next_token_invalid_empty))
    {
        return (1);
    }
    
    return (0);
}