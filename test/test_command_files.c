/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_command_files.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 21:05:22 by dlemaire          #+#    #+#             */
/*   Updated: 2025/04/29 21:41:55 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

// === Test helpers ===
void cleanup_file(const char *filename) {
	unlink(filename);
}

// === Tests ===

void test_create_new_file_node(void)
{
	t_minishell shell = {0};
	t_files *node = create_new_file_node(&shell);
	CU_ASSERT_PTR_NOT_NULL(node);
	CU_ASSERT_PTR_NULL(node->delim);
	CU_ASSERT_PTR_NULL(node->next);
}

void test_append_file_node_single(void)
{
	t_files *head = NULL;
	t_files node = { .fd = 3, .next = NULL };

	append_file_node(&head, &node);
	CU_ASSERT_PTR_EQUAL(head, &node);
	CU_ASSERT_PTR_NULL(head->next);
}

void test_append_file_node_chain(void)
{
	t_files a = { .fd = 1, .next = NULL };
	t_files b = { .fd = 2, .next = NULL };
	t_files *head = &a;

	append_file_node(&head, &b);
	CU_ASSERT_PTR_EQUAL(a.next, &b);
	CU_ASSERT_PTR_NULL(b.next);
}

void test_add_infile_to_cmd_valid(void)
{
	const char *filename = "test_input.txt";
	int fd = creat(filename, 0644);
	close(fd);

	t_minishell shell = {0};
	t_command cmd = {0};
	add_infile_to_cmd(&cmd, (char *)filename, &shell);

	CU_ASSERT_PTR_NOT_NULL(cmd.infile);
	CU_ASSERT(cmd.infile->fd >= 0);
	CU_ASSERT_EQUAL(shell.last_exit_status, 0);
	CU_ASSERT_EQUAL(cmd.redirect_error_flag, 0);

	close(cmd.infile->fd);
	cleanup_file(filename);
}

void test_add_infile_to_cmd_invalid(void)
{
	t_minishell shell = {0};
	t_command cmd = {0};

	add_infile_to_cmd(&cmd, "nonexistent_input.txt", &shell);

	CU_ASSERT_PTR_NOT_NULL(cmd.infile);
	CU_ASSERT(cmd.infile->fd >= 0);  // should fallback to /dev/null
	CU_ASSERT_EQUAL(shell.last_exit_status, 1);
	CU_ASSERT_EQUAL(cmd.redirect_error_flag, 1);

	close(cmd.infile->fd);
}

void test_add_outfile_to_cmd_truncate(void)
{
	const char *filename = "test_output.txt";
	t_minishell shell = {0};
	t_command cmd = {0};

	add_outfile_to_cmd(&cmd, (char *)filename, &shell, 0);

	CU_ASSERT_PTR_NOT_NULL(cmd.outfile);
	CU_ASSERT(cmd.outfile->fd >= 0);
	CU_ASSERT_EQUAL(shell.last_exit_status, 0);
	CU_ASSERT_EQUAL(cmd.redirect_error_flag, 0);

	close(cmd.outfile->fd);
	cleanup_file(filename);
}

void test_add_outfile_to_cmd_append(void)
{
	const char *filename = "test_output_append.txt";
	t_minishell shell = {0};
	t_command cmd = {0};

	add_outfile_to_cmd(&cmd, (char *)filename, &shell, 1);

	CU_ASSERT_PTR_NOT_NULL(cmd.outfile);
	CU_ASSERT(cmd.outfile->fd >= 0);
	CU_ASSERT_EQUAL(shell.last_exit_status, 0);
	CU_ASSERT_EQUAL(cmd.redirect_error_flag, 0);

	close(cmd.outfile->fd);
	cleanup_file(filename);
}

int add_command_files_tests(void)
{
    CU_pSuite suite = CU_add_suite("command_files", 0, 0);
    
    if (!suite)
        return (1);
    
    if (!CU_add_test(suite, "create_new_file_node", test_create_new_file_node) ||
        !CU_add_test(suite, "append_file_node (single)", test_append_file_node_single) ||
        !CU_add_test(suite, "append_file_node (chain)", test_append_file_node_chain) ||
        !CU_add_test(suite, "add_infile_to_cmd (valid file)", test_add_infile_to_cmd_valid) ||
        !CU_add_test(suite, "add_infile_to_cmd (invalid file)", test_add_infile_to_cmd_invalid) ||
        !CU_add_test(suite, "add_outfile_to_cmd (truncate)", test_add_outfile_to_cmd_truncate) ||
        !CU_add_test(suite, "add_outfile_to_cmd (append)", test_add_outfile_to_cmd_append))
    {
        return (1);
    }
    
    return (0);
}