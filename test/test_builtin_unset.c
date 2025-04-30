/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_builtin_pwd_env.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 21:05:22 by dlemaire          #+#    #+#             */
/*   Updated: 2025/04/29 21:41:55 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

int find_env_index_mock(const char *name, t_minishell *shell)
{
    // Mock implementation: Return an index or -1 for missing variable
    for (int i = 0; shell->env[i] != NULL; i++) {
        if (strncmp(name, shell->env[i], strlen(name)) == 0) {
            return i;
        }
    }
    return -1;
}

int unset_env_variable_mock(const char *name, t_minishell *shell)
{
    int index = find_env_index_mock(name, shell);
    if (index == -1) return 0; // No variable to unset
    for (int i = index; shell->env[i] != NULL; i++) {
        shell->env[i] = shell->env[i + 1];
    }
    return 0; // Successfully removed
}

void test_builtin_unset_valid(void)
{
    t_minishell shell;
    char *env_vars[] = {"VAR1=value1", "VAR2=value2", NULL};
    shell.env = env_vars;

    // Simulate an unset of VAR1
    char *args[] = {"unset", "VAR1", NULL};
    int result = builtin_unset(args, &shell);

    CU_ASSERT_EQUAL(result, 0); // No error should occur
    CU_ASSERT_EQUAL(find_env_index_mock("VAR1", &shell), -1); // VAR1 should be removed
    CU_ASSERT_EQUAL(find_env_index_mock("VAR2", &shell), 1); // VAR2 should remain
}

void test_builtin_unset_invalid_name(void)
{
    t_minishell shell;
    char *env_vars[] = {"VAR1=value1", "VAR2=value2", NULL};
    shell.env = env_vars;

    // Simulate an invalid unset
    char *args[] = {"unset", "1INVALID", NULL};
    int result = builtin_unset(args, &shell);

    CU_ASSERT_EQUAL(result, 1); // Should return error code for invalid variable name
}

void test_process_unset_argument_valid(void)
{
    t_minishell shell;
    char *env_vars[] = {"VAR1=value1", "VAR2=value2", NULL};
    shell.env = env_vars;

    // Process unset of a valid variable
    int result = process_unset_argument("VAR1", &shell);

    CU_ASSERT_EQUAL(result, 0); // Successful removal
    CU_ASSERT_EQUAL(find_env_index_mock("VAR1", &shell), -1); // VAR1 should be removed
}

void test_process_unset_argument_invalid(void)
{
    t_minishell shell;
    char *env_vars[] = {"VAR1=value1", "VAR2=value2", NULL};
    shell.env = env_vars;

    // Process unset of an invalid variable name
    int result = process_unset_argument("1INVALID", &shell);

    CU_ASSERT_EQUAL(result, 1); // Invalid name, should return error code
}

void test_is_valid_env_name_valid(void)
{
    CU_ASSERT_TRUE(is_valid_env_name("VAR1")); // Valid name
    CU_ASSERT_TRUE(is_valid_env_name("_VAR")); // Valid name with underscore
    CU_ASSERT_TRUE(is_valid_env_name("VAR_2")); // Valid name with underscore in middle
}

void test_is_valid_env_name_invalid(void)
{
    CU_ASSERT_FALSE(is_valid_env_name("1INVALID")); // Invalid, starts with number
    CU_ASSERT_FALSE(is_valid_env_name("VAR@1")); // Invalid, contains special character
    CU_ASSERT_FALSE(is_valid_env_name("VAR 2")); // Invalid, contains space
}

void test_find_env_index(void)
{
    t_minishell shell;
    char *env_vars[] = {"VAR1=value1", "VAR2=value2", NULL};
    shell.env = env_vars;

    // Find existing variables
    int index = find_env_index_mock("VAR1", &shell);
    CU_ASSERT_EQUAL(index, 0); // VAR1 is at index 0

    index = find_env_index_mock("VAR2", &shell);
    CU_ASSERT_EQUAL(index, 1); // VAR2 is at index 1

    // Find non-existing variable
    index = find_env_index_mock("VAR3", &shell);
    CU_ASSERT_EQUAL(index, -1); // VAR3 does not exist
}

void test_unset_env_variable(void)
{
    t_minishell shell;
    char *env_vars[] = {"VAR1=value1", "VAR2=value2", NULL};
    shell.env = env_vars;

    // Unset VAR1
    int result = unset_env_variable_mock("VAR1", &shell);
    CU_ASSERT_EQUAL(result, 0); // Should succeed
    CU_ASSERT_EQUAL(find_env_index_mock("VAR1", &shell), -1); // VAR1 should be removed

    // Unset non-existing variable
    result = unset_env_variable_mock("VAR3", &shell);
    CU_ASSERT_EQUAL(result, 0); // Should succeed but do nothing
}

int add_builtin_unset_tests(void)
{
    CU_pSuite suite = CU_add_suite("builtin_export", 0, 0);
    
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
