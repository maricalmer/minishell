/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_builtin_export.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 21:05:22 by dlemaire          #+#    #+#             */
/*   Updated: 2025/05/08 19:41:00 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "test.h"

extern int  process_export_argument(const char *arg, t_minishell *shell);
extern int  is_valid_identifier(const char *arg);
extern int  print_export_error(const char *arg);
extern int  builtin_export(char **args, t_minishell *shell);

void	test_builtin_export_without_args(void)
{
	t_minishell shell;

	init_shell_mock(&shell);
	char *args[] = { "export", NULL };
	int result = builtin_export(args, &shell);

	CU_ASSERT_EQUAL(result, 0);
}

void	test_is_valid_identifier_valid(void)
{
	CU_ASSERT_TRUE(is_valid_identifier("VAR"));
	CU_ASSERT_TRUE(is_valid_identifier("_VAR"));
	CU_ASSERT_TRUE(is_valid_identifier("VAR_123"));
}

void	test_is_valid_identifier_invalid(void)
{
	CU_ASSERT_FALSE(is_valid_identifier("123VAR"));
	CU_ASSERT_FALSE(is_valid_identifier("VAR$"));
	CU_ASSERT_FALSE(is_valid_identifier("VAR!"));
}

void	test_print_export_error(void)
{
	int result = print_export_error("1INVALID=val");
	CU_ASSERT_EQUAL(result, 1);
}

int	add_builtin_export_tests(void)
{
	CU_pSuite suite = CU_add_suite("builtin_export", 0, 0);
	if (!suite)
		return (1);
	if (!CU_add_test(suite, "test_builtin_export_without_args", test_builtin_export_without_args) ||
		!CU_add_test(suite, "test_is_valid_identifier_valid", test_is_valid_identifier_valid) ||
		!CU_add_test(suite, "test_is_valid_identifier_invalid", test_is_valid_identifier_invalid) ||
		!CU_add_test(suite, "test_print_export_error", test_print_export_error))
	{
		return (1);
	}
	return (0);
}
