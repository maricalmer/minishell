/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_lexer_state_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 21:05:22 by dlemaire          #+#    #+#             */
/*   Updated: 2025/05/08 20:07:46 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

static void	init_lexer_for_test(t_lexer *lexer, const char *str)
{
	lexer->str = str;
	lexer->pos = 0;
	lexer->current_char = str[0];
	lexer->state = DEFAULT_STATE;
}

void	test_handle_dollar_with_non_var_char(void)
{
	t_lexer lexer;
	t_minishell shell = {0};
	char *buffer = strdup("");
	init_lexer_for_test(&lexer, "$$");

	int result = handle_dollar_sign(&lexer, &buffer, &shell);

	CU_ASSERT_EQUAL(result, TOKEN_CONTINUE);
	CU_ASSERT_STRING_EQUAL(buffer, "$");
	CU_ASSERT_EQUAL(lexer.pos, 1);
	free(buffer);
}

void	test_handle_dollar_at_end_of_string(void)
{
	t_lexer lexer;
	t_minishell shell = {0};
	char *buffer = strdup("");
	init_lexer_for_test(&lexer, "$");

	int result = handle_dollar_sign(&lexer, &buffer, &shell);

	CU_ASSERT_EQUAL(result, TOKEN_CONTINUE);
	CU_ASSERT_STRING_EQUAL(buffer, "$");
	free(buffer);
}

void	test_advance_and_append_single_char(void)
{
	t_lexer lexer;
	t_minishell shell = {0};
	char *buffer = strdup("start");

	init_lexer_for_test(&lexer, "x");

	int result = advance_and_append(&lexer, &buffer, &shell);

	CU_ASSERT_EQUAL(result, 1);
	CU_ASSERT_STRING_EQUAL(buffer, "startx");
	free(buffer);
}

int	add_lexer_state_utils_tests(void)
{
    CU_pSuite suite = CU_add_suite("lexer_state_utils", 0, 0);
    
    if (!suite)
        return (1);
    if (!CU_add_test(suite, "test_handle_dollar_with_non_var_char", test_handle_dollar_with_non_var_char) ||
        !CU_add_test(suite, "test_handle_dollar_at_end_of_string", test_handle_dollar_at_end_of_string) ||
        !CU_add_test(suite, "test_advance_and_append_single_char", test_advance_and_append_single_char))
    {
        return (1);
    }
    return (0);
}