/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_env_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 21:05:22 by dlemaire          #+#    #+#             */
/*   Updated: 2025/04/29 21:41:55 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

extern int find_env_index(const char *name, t_minishell *shell); // Assume provided elsewhere

// int find_env_index_mock(const char *name, t_minishell *shell)
// {
// 	// Simple mock: only recognize "FOO"
//     (void)shell;
// 	if (strcmp(name, "FOO") == 0)
// 		return 0;
// 	return -1;
// }

void test_duplicate_env_basic(void)
{
	t_minishell shell = {0};
	char *envp[] = {
		"USER=student",
		"PATH=/usr/bin",
		NULL
	};

	char **copy = duplicate_env(envp, &shell);
	CU_ASSERT_PTR_NOT_NULL(copy);
	CU_ASSERT_STRING_EQUAL(copy[0], "USER=student");
	CU_ASSERT_STRING_EQUAL(copy[1], "PATH=/usr/bin");
	CU_ASSERT_PTR_NULL(copy[2]);
}

void test_create_env_string_basic(void)
{
	t_minishell shell = {0};
	char *result = create_env_string("FOO", "bar", &shell);
	CU_ASSERT_PTR_NOT_NULL(result);
	CU_ASSERT_STRING_EQUAL(result, "FOO=bar");
}

void test_add_env_variable_to_empty_env(void)
{
	t_minishell shell = {0};
	shell.env = gc_calloc(&shell.gc_head, 1, sizeof(char *));
	char *new_var = gc_strdup(&shell.gc_head, "FOO=123");

	int status = add_env_variable(new_var, &shell);
	CU_ASSERT_EQUAL(status, 0);
	CU_ASSERT_STRING_EQUAL(shell.env[0], "FOO=123");
	CU_ASSERT_PTR_NULL(shell.env[1]);
}

void test_set_env_variable_add(void)
{
	t_minishell shell = {0};
	shell.env = gc_calloc(&shell.gc_head, 1, sizeof(char *));  // Empty env
	int status = set_env_variable("FOO", "bar", &shell);
	CU_ASSERT_EQUAL(status, 0);
	CU_ASSERT_PTR_NOT_NULL(shell.env[0]);
	CU_ASSERT_STRING_EQUAL(shell.env[0], "FOO=bar");
}

void test_set_env_variable_override(void)
{
	t_minishell shell = {0};
	char *initial = gc_strdup(&shell.gc_head, "FOO=old");
	char *env[] = { initial, NULL };
	shell.env = env;

	int status = set_env_variable("FOO", "newval", &shell);
	CU_ASSERT_EQUAL(status, 0);
	CU_ASSERT_STRING_EQUAL(shell.env[0], "FOO=newval");
}

int add_env_utils_tests(void)
{
    CU_pSuite suite = CU_add_suite("builtin_export", 0, 0);
    
    if (!suite)
        return (1);
    
    if (!CU_add_test(suite, "test_duplicate_env_basic", test_duplicate_env_basic) ||
        !CU_add_test(suite, "test_create_env_string_basic", test_create_env_string_basic) ||
        !CU_add_test(suite, "test_add_env_variable_to_empty_env", test_add_env_variable_to_empty_env) ||
        !CU_add_test(suite, "test_set_env_variable_add", test_set_env_variable_add) ||
        !CU_add_test(suite, "test_set_env_variable_override", test_set_env_variable_override))
    {
        return (1);
    }
    
    return (0);
}