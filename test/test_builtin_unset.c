/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_builtin_unset.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 21:05:22 by dlemaire          #+#    #+#             */
/*   Updated: 2025/05/08 20:14:27 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

int	find_env_index_mock(const char *name, t_minishell *shell)
{
	int	i;

	i = 0;
	while (shell->env[i] != NULL)
	{
		if (strncmp(name, shell->env[i], strlen(name)) == 0)
			return (i);
		i++;
	}
	return (-1);
}

int	unset_env_variable_mock(const char *name, t_minishell *shell)
{
	int	i;

	i = find_env_index_mock(name, shell);
	if (i == -1)
		return (0);
	while (shell->env[i] != NULL)
	{
		shell->env[i] = shell->env[i + 1];
		i++;
	}
	return (0);
}

void	test_builtin_unset_valid(void)
{
	t_minishell shell = create_mock_shell(42, 0);
	char *env_vars[] = {"VAR1=value1", "VAR2=value2", NULL};
	shell.env = env_vars;

	char *args[] = {"unset", "VAR1", NULL};
	int result = builtin_unset(args, &shell);

	CU_ASSERT_EQUAL(result, 0);
	CU_ASSERT_EQUAL(find_env_index_mock("VAR1", &shell), -1);
	CU_ASSERT_EQUAL(find_env_index_mock("VAR2", &shell), 1);
}

void	test_builtin_unset_invalid_name(void)
{
	t_minishell shell = create_mock_shell(42, 0);
	char *env_vars[] = {"VAR1=value1", "VAR2=value2", NULL};
	shell.env = env_vars;

	char *args[] = {"unset", "1INVALID", NULL};
	int result = builtin_unset(args, &shell);

	CU_ASSERT_EQUAL(result, 1);
}

void	test_process_unset_argument_valid(void)
{
	t_minishell shell = create_mock_shell(42, 0);
	char *env_vars[] = {"VAR1=value1", "VAR2=value2", NULL};
	shell.env = env_vars;

	int result = process_unset_argument("VAR1", &shell);

	CU_ASSERT_EQUAL(result, 0);
	CU_ASSERT_EQUAL(find_env_index_mock("VAR1", &shell), -1);
}

void	test_process_unset_argument_invalid(void)
{
	t_minishell shell = create_mock_shell(42, 0);
	char *env_vars[] = {"VAR1=value1", "VAR2=value2", NULL};
	shell.env = env_vars;

	int result = process_unset_argument("1INVALID", &shell);
	CU_ASSERT_EQUAL(result, 1);
}

void	test_is_valid_env_name_valid(void)
{
	CU_ASSERT_TRUE(is_valid_env_name("VAR1"));
	CU_ASSERT_TRUE(is_valid_env_name("_VAR"));
	CU_ASSERT_TRUE(is_valid_env_name("VAR_2"));
}

void	test_is_valid_env_name_invalid(void)
{
	CU_ASSERT_FALSE(is_valid_env_name("1INVALID"));
	CU_ASSERT_FALSE(is_valid_env_name("VAR@1"));
	CU_ASSERT_FALSE(is_valid_env_name("VAR 2"));
}

void	test_find_env_index(void)
{
	t_minishell shell = create_mock_shell(42, 0);
	char *env_vars[] = {"VAR1=value1", "VAR2=value2", NULL};
	shell.env = env_vars;

	int index = find_env_index_mock("VAR1", &shell);
	CU_ASSERT_EQUAL(index, 0);

	index = find_env_index_mock("VAR2", &shell);
	CU_ASSERT_EQUAL(index, 1);

	index = find_env_index_mock("VAR3", &shell);
	CU_ASSERT_EQUAL(index, -1);
}

void test_unset_env_variable(void)
{
	t_minishell shell = create_mock_shell(42, 0);
	char *env_vars[] = {"VAR1=value1", "VAR2=value2", NULL};
	shell.env = env_vars;

	int result = unset_env_variable_mock("VAR1", &shell);
	CU_ASSERT_EQUAL(result, 0);
	CU_ASSERT_EQUAL(find_env_index_mock("VAR1", &shell), -1);

	result = unset_env_variable_mock("VAR3", &shell);
	CU_ASSERT_EQUAL(result, 0);
}

int add_builtin_unset_tests(void)
{
	CU_pSuite suite = CU_add_suite("builtin_unset", 0, 0);

	if (!suite)
		return (1);
	if (!CU_add_test(suite, "test_builtin_unset_valid", test_builtin_unset_valid) ||
		!CU_add_test(suite, "test_builtin_unset_invalid_name", test_builtin_unset_invalid_name) ||
		!CU_add_test(suite, "test_process_unset_argument_valid", test_process_unset_argument_valid) ||
		!CU_add_test(suite, "test_process_unset_argument_invalid", test_process_unset_argument_invalid) ||
		!CU_add_test(suite, "test_is_valid_env_name_valid", test_is_valid_env_name_valid) ||
		!CU_add_test(suite, "test_is_valid_env_name_invalid", test_is_valid_env_name_invalid) ||
		!CU_add_test(suite, "test_find_env_index", test_find_env_index) ||
		!CU_add_test(suite, "test_unset_env_variable", test_unset_env_variable))
	{
		return (1);
	}
	return (0);
}
