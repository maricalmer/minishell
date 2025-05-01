/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_lexer_main.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 21:05:22 by dlemaire          #+#    #+#             */
/*   Updated: 2025/04/29 21:41:55 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

void test_init_lexer(void) {
    const char *input = "echo hello";
    t_lexer lexer = init_lexer(input);

    CU_ASSERT_PTR_EQUAL(lexer.str, input);
    CU_ASSERT_EQUAL(lexer.pos, 0);
    CU_ASSERT_EQUAL(lexer.current_char, 'e');
    CU_ASSERT_EQUAL(lexer.state, DEFAULT_STATE);
    CU_ASSERT_EQUAL(lexer.command_expected, 1);
}

void test_initialize_lexer_and_tokens(void) {
    char *input = "ls -la";
    t_minishell shell;
    shell.tokens = (t_token *)0xdeadbeef;  // intentionally non-null to ensure reset
    t_lexer lexer;

    initialize_lexer_and_tokens(input, &lexer, &shell);

    CU_ASSERT_PTR_EQUAL(lexer.str, input);
    CU_ASSERT_EQUAL(lexer.pos, 0);
    CU_ASSERT_EQUAL(lexer.current_char, 'l');
    CU_ASSERT_EQUAL(shell.tokens, NULL);
}

void test_run_lexer_executes_all(void) {
    char *input = "cat file.txt";
    t_minishell shell;
    shell.tokens = NULL;

    run_lexer(input, &shell);

    // Check only that function ran without crashing.
    // Optionally, assert lexer logic depending on actual processing behavior
    CU_PASS("run_lexer executed without crashing");
}

int add_lexer_main_tests(void)
{
    CU_pSuite suite = CU_add_suite("lexer_main", 0, 0);
    
    if (!suite)
        return (1);
    
    if (!CU_add_test(suite, "test_init_lexer", test_init_lexer) ||
        !CU_add_test(suite, "test_initialize_lexer_and_tokens", test_initialize_lexer_and_tokens) ||
        !CU_add_test(suite, "test_run_lexer_executes_all", test_run_lexer_executes_all))
    {
        return (1);
    }
    
    return (0);
}