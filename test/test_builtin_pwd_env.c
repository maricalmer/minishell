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

void test_builtin_pwd(void)
{
    t_command cmd;

    // Test when current working directory is retrieved successfully
    char *pwd_args[] = {"pwd", NULL};
    cmd.args = pwd_args;
    cmd.outfile = NULL;  // No file redirection
    int status = builtin_pwd(&cmd);
    CU_ASSERT_EQUAL(status, 0); // Should return 0 on success

    // Check if it prints the current working directory (mocking ft_putendl_fd)
    // You can use a function like `ft_putstr_fd` to capture output in a buffer for testing
}

// Test builtin_env
void test_builtin_env(void)
{
    t_command cmd;
    t_minishell shell;
    char *env_vars[] = {"VAR1=value1", "VAR2=value2", NULL};
    shell.env = env_vars;  // Mock environment variables

    // Test without too many arguments
    char *env_args[] = {"env", NULL};
    cmd.args = env_args;
    cmd.outfile = NULL;  // No file redirection
    int status = builtin_env(&cmd, &shell);
    CU_ASSERT_EQUAL(status, 0); // Should return 0 on success

    // Test with too many arguments
    char *env_args_too_many[] = {"env", "extra", NULL};
    cmd.args = env_args_too_many;
    status = builtin_env(&cmd, &shell);
    CU_ASSERT_EQUAL(status, 127); // Should return 127 for too many arguments
}

// Test too_many_arguments
void test_too_many_arguments(void)
{
    char *args_valid[] = {"env", NULL};
    char *args_invalid[] = {"env", "extra", NULL};

    int status = too_many_arguments(args_valid);
    CU_ASSERT_EQUAL(status, 0); // Should return 0 if valid number of arguments

    status = too_many_arguments(args_invalid);
    CU_ASSERT_EQUAL(status, 1); // Should return 1 and print error for too many arguments
}

// Test cd_too_many_arguments
void test_cd_too_many_arguments(void)
{
    char *args_valid[] = {"cd", "/home", NULL};
    char *args_invalid[] = {"cd", "/home", "extra", NULL};

    int status = cd_too_many_arguments(args_valid);
    CU_ASSERT_EQUAL(status, 0); // Should return 0 for valid number of arguments

    status = cd_too_many_arguments(args_invalid);
    CU_ASSERT_EQUAL(status, 1); // Should return 1 and print error for too many arguments
}


int add_builtin_pwd_env_tests(void)
{
    CU_pSuite suite = CU_add_suite("builtin_export", 0, 0);
    
    if (!suite)
        return (1);
    
    if (!CU_add_test(suite, "test_builtin_pwd", test_builtin_pwd) ||
        !CU_add_test(suite, "test_builtin_env", test_builtin_env) ||
        !CU_add_test(suite, "test_too_many_arguments", test_too_many_arguments) ||
        !CU_add_test(suite, "test_cd_too_many_arguments", test_cd_too_many_arguments))
    {
        return (1);
    }
    
    return (0);
}
