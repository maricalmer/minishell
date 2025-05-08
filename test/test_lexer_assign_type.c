/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_lexer_assign_type.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 21:05:22 by dlemaire          #+#    #+#             */
/*   Updated: 2025/05/08 20:17:00 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

void	test_is_special_character_basic(void)
{
	t_lexer	lexer;

	lexer.str = "|";
	lexer.pos = 0;
	lexer.current_char = lexer.str[0];
	CU_ASSERT_TRUE(is_special_character(&lexer));

	lexer.str = ">";
	lexer.pos = 0;
	lexer.current_char = lexer.str[0];
	CU_ASSERT_TRUE(is_special_character(&lexer));

	lexer.str = "&";
	lexer.pos = 0;
	lexer.current_char = lexer.str[0];
	CU_ASSERT_TRUE(is_special_character(&lexer));

	lexer.str = "a";
	lexer.pos = 0;
	lexer.current_char = lexer.str[0];
	CU_ASSERT_FALSE(is_special_character(&lexer));
}

void	test_is_special_character_double_chars(void)
{
	t_lexer	lexer;

	lexer.str = "&&";
	lexer.pos = 0;
	lexer.current_char = lexer.str[0];
	CU_ASSERT_TRUE(is_special_character(&lexer));

	lexer.str = "||";
	lexer.pos = 0;
	lexer.current_char = lexer.str[0];
	CU_ASSERT_TRUE(is_special_character(&lexer));

	lexer.str = "<<";
	lexer.pos = 0;
	lexer.current_char = lexer.str[0];
	CU_ASSERT_TRUE(is_special_character(&lexer));

	lexer.str = ">>";
	lexer.pos = 0;
	lexer.current_char = lexer.str[0];
	CU_ASSERT_TRUE(is_special_character(&lexer));
}

void	test_get_special_character_token_type(void)
{
	CU_ASSERT_EQUAL(get_special_character_token_type("|"), TOKEN_PIPE);
	CU_ASSERT_EQUAL(get_special_character_token_type("||"), TOKEN_OR);
	CU_ASSERT_EQUAL(get_special_character_token_type("&"), TOKEN_INVALID);
	CU_ASSERT_EQUAL(get_special_character_token_type("&&"), TOKEN_AND);
	CU_ASSERT_EQUAL(get_special_character_token_type("<"), TOKEN_REDIRECT_IN);
	CU_ASSERT_EQUAL(get_special_character_token_type(">"), TOKEN_REDIRECT_OUT);
	CU_ASSERT_EQUAL(get_special_character_token_type("<<"), TOKEN_HEREDOC);
	CU_ASSERT_EQUAL(get_special_character_token_type(">>"), TOKEN_REDIRECT_APPEND);
	CU_ASSERT_EQUAL(get_special_character_token_type("("), TOKEN_OP_PARENTHESIS);
	CU_ASSERT_EQUAL(get_special_character_token_type(")"), TOKEN_CL_PARENTHESIS);
	CU_ASSERT_EQUAL(get_special_character_token_type("?"), TOKEN_INVALID);
}

void	test_collect_special_character_single_char(void)
{
	t_lexer lexer;
	t_minishell shell;
	char *value;

	lexer.str = "<";
	lexer.pos = 0;
	lexer.current_char = lexer.str[0];
	shell.gc_head = NULL;
	value = collect_special_character(&lexer, &shell);
	CU_ASSERT_STRING_EQUAL(value, "<");
	free(value);
}

void	test_collect_special_character_double_char(void)
{
	t_lexer lexer;
	t_minishell shell;
	char *value;

	lexer.str = ">>file";
	lexer.pos = 0;
	lexer.current_char = lexer.str[0];
	shell.gc_head = NULL;
	value = collect_special_character(&lexer, &shell);
	CU_ASSERT_STRING_EQUAL(value, ">>");
	free(value);
}

int	add_lexer_assign_type_tests(void)
{
	CU_pSuite suite = CU_add_suite("lexer_assign_type", 0, 0);

	if (!suite)
		return (1);
	if (!CU_add_test(suite, "test_is_special_character_basic", test_is_special_character_basic) ||
		!CU_add_test(suite, "test_is_special_character_double_chars", test_is_special_character_double_chars) ||
		!CU_add_test(suite, "test_get_special_character_token_type", test_get_special_character_token_type) ||
		!CU_add_test(suite, "test_collect_special_character_single_char", test_collect_special_character_single_char) ||
		!CU_add_test(suite, "test_collect_special_character_double_char", test_collect_special_character_double_char))
	{
		return (1);
	}
	return (0);
}
