/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_command_init.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 21:05:22 by dlemaire          #+#    #+#             */
/*   Updated: 2025/04/29 21:41:55 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

void test_count_arg_tokens(void) {
    t_token t3 = {.type = TOKEN_ARGUMENT, .next = NULL};
    t_token t2 = {.type = TOKEN_EXTERN_CMD, .next = &t3};
    t_token t1 = {.type = TOKEN_ARGUMENT, .next = &t2};

    int result = count_arg_tokens(&t1);
    CU_ASSERT_EQUAL(result, 2);
}

void test_initialize_command_struct(void) {
    t_command cmd;
    cmd.cmd_name = (char *)1;
    cmd.path = (char *)1;
    cmd.infile = (void *)1;
    cmd.outfile = (void *)1;

    initialize_command_struct(&cmd);

    CU_ASSERT_PTR_NULL(cmd.cmd_name);
    CU_ASSERT_PTR_NULL(cmd.path);
    CU_ASSERT_PTR_NULL(cmd.infile);
    CU_ASSERT_PTR_NULL(cmd.outfile);
}

// Integration-style test
void test_init_command_basic(void) {
    t_token tok1 = {.type = TOKEN_ARGUMENT, .value = "arg1", .next = NULL};
    t_command cmd = {0};
    t_minishell shell = {0};

    init_command(&cmd, &tok1, &shell);

    CU_ASSERT_PTR_NOT_NULL(cmd.args);
    CU_ASSERT_STRING_EQUAL(cmd.args[0], "arg1");
    CU_ASSERT_PTR_NULL(cmd.args[1]);
    CU_ASSERT_EQUAL(cmd.redirect_error_flag, 0);
    CU_ASSERT_EQUAL(cmd.has_heredoc, 0);
    CU_ASSERT_PTR_EQUAL(shell.cmd, &cmd);
}

int add_command_init_tests(void)
{
    CU_pSuite suite = CU_add_suite("env_utils", 0, 0);
    
    if (!suite)
        return (1);
    
    if (!CU_add_test(suite, "test_count_arg_tokens", test_count_arg_tokens) ||
        !CU_add_test(suite, "test_initialize_command_struct", test_initialize_command_struct) ||
        !CU_add_test(suite, "test_init_command_basic", test_init_command_basic))
    {
        return (1);
    }
    
    return (0);
}