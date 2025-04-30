/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_builtin_exit.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 21:05:22 by dlemaire          #+#    #+#             */
/*   Updated: 2025/04/29 21:41:55 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

int mock_exit_called = 0;
int mock_exit_status = -1;

void exit(int status) {
	mock_exit_called = 1;
	mock_exit_status = status;
	while (1) {} // prevent real exit
}

void rl_clear_history(void) {}

// ---- Helpers ----
t_minishell create_mock_shell(int last_status, int in_pipe) {
	t_minishell shell = {
		.env = NULL,
		.last_exit_status = last_status,
		.in_pipe = in_pipe,
		.gc_head = NULL,
		.tokens = NULL,
		.ast_root = NULL,
		.cmd = NULL,
		.heredocs = NULL,
		.sigint_heredocs = 0
	};
	return shell;
}

// ---- Tests ----

void test_exit_no_args_returns_last_status(void) {
	t_minishell shell = create_mock_shell(42, 0);
	char *args[] = { "exit", NULL };

	int result = handle_exit_with_arguments(args, &shell);

	CU_ASSERT_EQUAL(result, 42);
}

void test_exit_numeric_argument(void) {
	t_minishell shell = create_mock_shell(0, 0);
	char *args[] = { "exit", "5", NULL };

	int result = handle_exit_with_arguments(args, &shell);

	CU_ASSERT_EQUAL(result, 5);
}

void test_exit_non_numeric_argument(void) {
	t_minishell shell = create_mock_shell(0, 0);
	char *args[] = { "exit", "abc", NULL };

	int result = handle_exit_with_arguments(args, &shell);

	CU_ASSERT_EQUAL(result, 2);
}

void test_exit_too_many_arguments_with_valid_numeric_first(void) {
	t_minishell shell = create_mock_shell(0, 0);
	char *args[] = { "exit", "1", "extra", NULL };

	int result = handle_exit_with_arguments(args, &shell);

	CU_ASSERT_EQUAL(result, -1);
	CU_ASSERT_EQUAL(shell.last_exit_status, 1);
}

void test_exit_too_many_arguments_with_invalid_first(void) {
	t_minishell shell = create_mock_shell(0, 0);
	char *args[] = { "exit", "abc", "extra", NULL };

	int result = handle_exit_with_arguments(args, &shell);

	CU_ASSERT_EQUAL(result, 2);
	CU_ASSERT_EQUAL(shell.last_exit_status, 2);
}

void test_is_numeric_argument(void) {
	CU_ASSERT_EQUAL(is_numeric_argument("123"), 1);
	CU_ASSERT_EQUAL(is_numeric_argument("-42"), 1);
	CU_ASSERT_EQUAL(is_numeric_argument("+99"), 1);
	CU_ASSERT_EQUAL(is_numeric_argument("12a"), 0);
	CU_ASSERT_EQUAL(is_numeric_argument(""), 0);
	CU_ASSERT_EQUAL(is_numeric_argument("   "), 0);
}

// ---- Register ----
int add_builtin_exit_tests(void) {
	CU_pSuite suite = CU_add_suite("builtin_exit", 0, 0);
	if (!suite)
		return 1;

	if (
		!CU_add_test(suite, "exit with no args returns last status", test_exit_no_args_returns_last_status) ||
		!CU_add_test(suite, "exit with numeric argument", test_exit_numeric_argument) ||
		!CU_add_test(suite, "exit with non-numeric argument", test_exit_non_numeric_argument) ||
		!CU_add_test(suite, "exit with too many args (valid first)", test_exit_too_many_arguments_with_valid_numeric_first) ||
		!CU_add_test(suite, "exit with too many args (invalid first)", test_exit_too_many_arguments_with_invalid_first) ||
		!CU_add_test(suite, "is_numeric_argument", test_is_numeric_argument)
	)
		return 1;

	return 0;
}

