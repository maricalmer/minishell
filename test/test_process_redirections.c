/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_process_redirections.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 21:05:22 by dlemaire          #+#    #+#             */
/*   Updated: 2025/04/29 21:41:55 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

t_minishell shell;
static jmp_buf env;

static void setup_shell_env(void) {
    shell.last_exit_status = 0;
    shell.gc_head = NULL;
    shell.tokens = NULL;
    shell.ast_root = NULL;
    shell.cmd = NULL;
    shell.heredocs = NULL;
    shell.sigint_heredocs = 0;
    shell.env = (char *[]){"PATH=/bin:/usr/bin", NULL};
}

void catch_exit(int status) {
    longjmp(env, status);
}

// Test for get_last_file function
void test_get_last_file(void) {
    t_files file1 = { .fd = 1, .next = NULL };
    t_files file2 = { .fd = 2, .next = NULL };
    t_files file3 = { .fd = 3, .next = NULL };

    // Single node test
    CU_ASSERT_PTR_EQUAL(get_last_file(&file1), &file1);

    // Multiple nodes test
    file1.next = &file2;
    file2.next = &file3;
    CU_ASSERT_PTR_EQUAL(get_last_file(&file1), &file3);

    // Empty list test
    CU_ASSERT_PTR_EQUAL(get_last_file(NULL), NULL);
}

int add_process_redirections_tests(void)
{
    CU_pSuite suite = CU_add_suite("process_redirections", 0, 0);
    
    if (!suite)
        return (1);
    
    if (!CU_add_test(suite, "test_get_last_file", test_get_last_file))
    {
        return (1);
    }
    
    return (0);
}