/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_heredoc_handler.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dlemaire <dlemaire@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 21:05:22 by dlemaire          #+#    #+#             */
/*   Updated: 2025/04/29 21:41:55 by dlemaire         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

void test_get_delimiter_basic(void) {
	t_minishell shell = {0};
	char *input = "EOF rest";
	char *result = get_delimiter(input, &shell);

	CU_ASSERT_STRING_EQUAL(result, "EOF");
	free(result);
}

void test_get_delimiter_with_symbols(void) {
	t_minishell shell = {0};
	char *input = "DELIM|next";
	char *result = get_delimiter(input, &shell);

	CU_ASSERT_STRING_EQUAL(result, "DELIM");
	free(result);
}

void test_is_quoted_delimiter_detects_quotes(void) {
	CU_ASSERT_EQUAL(is_quoted_delimiter("'DELIM'"), 1);
	CU_ASSERT_EQUAL(is_quoted_delimiter("\"DELIM\""), 1);
	CU_ASSERT_EQUAL(is_quoted_delimiter("DELIM"), 0);
	CU_ASSERT_EQUAL(is_quoted_delimiter(NULL), 0);
}

void test_handle_pipe_token_resets_list(void) {
	char *input = strdup("&&");
	t_list *current = (t_list *)0x1234; // dummy non-null
	handle_pipe_token(&input, &current);

	CU_ASSERT_PTR_NULL(current);
	CU_ASSERT_PTR_NOT_NULL(input); // was incremented
	free(input);
}

void test_handle_heredoc_token_adds_heredoc(void) {
	char *input = strdup("<<DELIM more");
	t_minishell shell = {0};
	t_list *outer = NULL;
	t_list *current = NULL;

	handle_heredoc_token_in_input(&input, &shell, &outer, &current);

	CU_ASSERT_PTR_NOT_NULL(outer);
	CU_ASSERT_PTR_NOT_NULL(current);
	CU_ASSERT_PTR_EQUAL(outer, current);
	CU_ASSERT_PTR_NOT_NULL(current->content);

	t_files *f = (t_files *)current->content;
	CU_ASSERT_STRING_EQUAL(f->delim, "DELIM");

	free(f->delim);
	free(f);
	free(current);
	free(input - 2);  // step back to original malloc ptr
}

void test_heredoc_scan_adds_heredocs_and_handles_pipes(void) {
	char input[] = "<<DELIM1 | <<'DELIM2'";
	t_minishell shell = {0};

	heredoc_scan(input, &shell);

	CU_ASSERT_PTR_NOT_NULL(shell.heredocs);
	t_list *first = shell.heredocs;
	CU_ASSERT_PTR_NOT_NULL(first->content);

	t_list *second = first->next;
	CU_ASSERT_PTR_NOT_NULL(second);
	CU_ASSERT_PTR_NOT_NULL(second->content);

	t_files *f1 = (t_files *)first->content;
	t_files *f2 = (t_files *)second->content;
	CU_ASSERT_STRING_EQUAL(f1->delim, "DELIM1");
	CU_ASSERT_STRING_EQUAL(f2->delim, "'DELIM2'");

	// Cleanup
	free(f1->delim);
	free(f2->delim);
	free(f1);
	free(f2);
	free(first);
	free(second);
}

int add_heredoc_handler_tests(void)
{
    CU_pSuite suite = CU_add_suite("heredoc_handler", 0, 0);
    
    if (!suite)
        return (1);
    
    if (!CU_add_test(suite, "test_get_delimiter_basic", test_get_delimiter_basic) ||
        !CU_add_test(suite, "test_get_delimiter_with_symbols", test_get_delimiter_with_symbols) ||
        !CU_add_test(suite, "test_is_quoted_delimiter_detects_quotes", test_is_quoted_delimiter_detects_quotes) ||
        !CU_add_test(suite, "test_handle_heredoc_token_adds_heredoc", test_handle_heredoc_token_adds_heredoc) ||
        !CU_add_test(suite, "test_heredoc_scan_adds_heredocs_and_handles_pipes", test_heredoc_scan_adds_heredocs_and_handles_pipes))
    {
        return (1);
    }
    return (0);
}