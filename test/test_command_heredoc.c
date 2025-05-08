/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_command_heredoc.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 21:05:22 by dlemaire          #+#    #+#             */
/*   Updated: 2025/05/08 20:15:30 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

void    test_handle_heredoc_token_with_delimiter(void)
{
	t_token t3 = { .next = NULL };
	t_token t2 = { .type = TOKEN_HEREDOC_DELIMITER, .value = "DELIM", .next = &t3 };
	t_token t1 = { .next = &t2 };
	t_command cmd = {0};
	t_minishell shell = {0};
	t_token *result = handle_heredoc_token(&cmd, &t1, &shell);

	CU_ASSERT_EQUAL(result, &t3);
	CU_ASSERT_PTR_NOT_NULL(cmd.infile);
	CU_ASSERT_STRING_EQUAL(cmd.infile->delim, "DELIM");
	CU_ASSERT_EQUAL(cmd.has_heredoc, 1);
}

void    test_handle_heredoc_token_without_delimiter(void)
{
	t_token t1 = { .next = NULL };
	t_command cmd = {0};
	t_minishell shell = {0};
	t_token *result = handle_heredoc_token(&cmd, &t1, &shell);

	CU_ASSERT_EQUAL(result, NULL);
	CU_ASSERT_PTR_NULL(cmd.infile);
}

void    test_add_heredoc_to_cmd_appends_correctly(void)
{
	t_minishell shell = {0};
	t_command cmd = {0};
	t_files hd = {.fd = 42, .next = NULL};
	t_list node = { .content = &hd, .next = NULL };

	shell.heredocs = &node;
	add_heredoc_to_cmd(&cmd, "EOF", &shell);
	CU_ASSERT_EQUAL(cmd.has_heredoc, 1);
	CU_ASSERT_PTR_NOT_NULL(cmd.infile);
	CU_ASSERT_STRING_EQUAL(cmd.infile->delim, "EOF");
	CU_ASSERT_EQUAL(cmd.infile->fd, 42);
}

void    test_get_last_heredoc_fd_returns_fd(void)
{
	t_files file1 = {.fd = 5, .next = NULL};
	t_list node1 = {.content = &file1, .next = NULL};

	int fd = get_last_heredoc_fd(&node1);
	CU_ASSERT_EQUAL(fd, 5);
}

void	test_get_last_heredoc_fd_empty(void)
{
	int fd = get_last_heredoc_fd(NULL);
	CU_ASSERT_EQUAL(fd, -1);
}

int add_command_heredoc_tests(void)
{
	CU_pSuite suite = CU_add_suite("command_files", 0, 0);
	
	if (!suite)
		return (1);
	if (!CU_add_test(suite, "test_handle_heredoc_token_with_delimiter", test_handle_heredoc_token_with_delimiter) ||
		!CU_add_test(suite, "test_handle_heredoc_token_without_delimiter", test_handle_heredoc_token_without_delimiter) ||
		!CU_add_test(suite, "test_add_heredoc_to_cmd_appends_correctly", test_add_heredoc_to_cmd_appends_correctly) ||
		!CU_add_test(suite, "test_get_last_heredoc_fd_returns_fd", test_get_last_heredoc_fd_returns_fd) ||
		!CU_add_test(suite, "test_get_last_heredoc_fd_empty", test_get_last_heredoc_fd_empty))
	{
		return (1);
	}
	return (0);
}
