/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_variable_expansion.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 21:05:22 by dlemaire          #+#    #+#             */
/*   Updated: 2025/04/29 21:41:55 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

void test_get_variable_name_length_basic(void) {
	CU_ASSERT_EQUAL(get_variable_name_length("VAR1"), 4);
	CU_ASSERT_EQUAL(get_variable_name_length("VAR_2"), 5);
	CU_ASSERT_EQUAL(get_variable_name_length("VAR!"), 3); // stops at !
	CU_ASSERT_EQUAL(get_variable_name_length("?"), 1);
	CU_ASSERT_EQUAL(get_variable_name_length(""), 0);
}

void test_get_variable_value_existing(void) {
	t_minishell shell = {0};
	char *env[] = {
		"USER=testuser",
		"PATH=/usr/bin",
		NULL
	};
	shell.env = env;
	CU_ASSERT_STRING_EQUAL(get_variable_value("USER", &shell), "testuser");
	CU_ASSERT_STRING_EQUAL(get_variable_value("PATH", &shell), "/usr/bin");
}

void test_get_variable_value_nonexistent(void) {
	t_minishell shell = {0};
	char *env[] = {
		"USER=testuser",
		NULL
	};
	shell.env = env;
	CU_ASSERT_STRING_EQUAL(get_variable_value("DOES_NOT_EXIST", &shell), "");
}

void test_get_variable_value_exit_status(void) {
	t_minishell shell = {0};
	shell.last_exit_status = 42;
	char *val = get_variable_value("?", &shell);
	CU_ASSERT_PTR_NOT_NULL(val);
	CU_ASSERT_STRING_EQUAL(val, "42");
}

void test_append_to_buffer_success(void) {
	t_minishell shell = {0};
	shell.gc_head = NULL; // assuming GC can start null
	char *buffer = gc_strdup(&shell.gc_head, "Hello");
	int result = append_to_buffer(&buffer, " World", &shell);
	CU_ASSERT_EQUAL(result, 1);
	CU_ASSERT_STRING_EQUAL(buffer, "Hello World");
}

void test_collect_variable_name_basic(void) {
	t_minishell shell = {0};
	t_lexer lexer = {
		.str = "VAR1 rest",
		.pos = 0,
		.current_char = 'V'
	};
	char *name = collect_variable_name(&lexer, &shell);
	CU_ASSERT_STRING_EQUAL(name, "VAR1");
	CU_ASSERT_EQUAL(lexer.pos, 4);
}

void test_collect_variable_name_exit_status(void) {
	t_minishell shell = {0};
	t_lexer lexer = {
		.str = "? more",
		.pos = 0,
		.current_char = '?'
	};
	char *name = collect_variable_name(&lexer, &shell);
	CU_ASSERT_STRING_EQUAL(name, "?");
}

void test_handle_variable_expansion_valid(void) {
	t_minishell shell = {0};
	char *env[] = {"VAR1=HelloWorld", NULL};
	shell.env = env;
	t_lexer lexer = {
		.str = "$VAR1",
		.pos = 0,
		.current_char = '$'
	};
	char *buffer = gc_strdup(&shell.gc_head, "");
	int status = handle_variable_expansion(&lexer, &buffer, &shell);
	CU_ASSERT_EQUAL(status, 1);
	CU_ASSERT_STRING_EQUAL(buffer, "HelloWorld");
}

int add_variable_expansion_tests(void)
{
    CU_pSuite suite = CU_add_suite("var_expansion", 0, 0);
    
    if (!suite)
        return (1);
    
    if (!CU_add_test(suite, "test_get_variable_name_length_basic", test_get_variable_name_length_basic) ||
        !CU_add_test(suite, "test_get_variable_value_existing", test_get_variable_value_existing) ||
        !CU_add_test(suite, "test_get_variable_value_nonexistent", test_get_variable_value_nonexistent) ||
        !CU_add_test(suite, "test_get_variable_value_exit_status", test_get_variable_value_exit_status) ||
        !CU_add_test(suite, "test_append_to_buffer_success", test_append_to_buffer_success) ||
        !CU_add_test(suite, "test_collect_variable_name_basic", test_collect_variable_name_basic) ||
        !CU_add_test(suite, "test_collect_variable_name_exit_status", test_collect_variable_name_exit_status) ||
        !CU_add_test(suite, "test_handle_variable_expansion_valid", test_handle_variable_expansion_valid))
    {
        return (1);
    }
    
    return (0);
}